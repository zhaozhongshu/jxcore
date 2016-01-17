### Evented I/O for (SpiderMonkey, V8, and..) JavaScript

JXcore extends Node.JS™ with additional features for developing mobile and embedded applications using JavaScript and leveraging the Node ecosystem.

Master build status: [![Build Status](https://travis-ci.org/jxcore/jxcore.svg?branch=master)](https://travis-ci.org/jxcore/jxcore)

### Features include

  - Support for core Node.JS features
  - Embeddable Interface
  - Publish to Mobile Platforms (Android, iOS ..)
  - Supports Multiple JavaScript Engines
  - Multi-threading Capabilities
  - Process Configuration & Monitor
  - In-memory File System
  - Application Packaging
  - Support for the latest JavaScript features (ES6, [ASM.JS](https://github.com/jxcore/jxcore/blob/master/doc/api/globals.markdown#require) ...)

### Which architectures are supported

  - ARMV7, ARMV7a, ARMv8 (64 bit**)
  - MIPS
  - ia32, X64
  
Coming soon;
  - Power7, Power8, MIPS (64 bit)

### Which platforms are supported

  - Android 2.3+
  - CyanogenMod 10+
  - iOS 6+ 
  - Windows XP+
  - Various Linux Distributions (Debian, Fedora, Suse, OpenWRT, Gentoo ...)
  - BSD 
  - OSX 

### Test environment

  - Ubuntu 14 LTS VM
  - Debian 7 VM, Ci20(mips), RP B+(arm), RP 2(arm), VM(osuosl - power8)
  - Udoo (arm + arduino)
  - FreeBSD 10 VM
  - Windows 7 x64, 8.1 x64
  - OSX 10.9, 10.10 
  - Android 2.3 Samsung ACE
  - Android 3.x Samsung Galaxy Tab2 
  - Android 4.x Huawei G510 /LG G2 / HTC 820(armv8) / Dell Venue 7 / Ci20(mips)
  - Android 5.x Nexus 5 / Nexus 6 / S6
  - Cyanogen 11+ one plus one 
  - iOS 6.x iPhone 4s
  - iOS 7.x iPad Mini 2nd, IPhone 5s
  - iOS 8.x iPhone 6
  - OpenWRT VM, Yun(mips)
  
### Roadmap

See [Roadmap](doc/ROADMAP.md) document.

### To build:

See [How to Compile](doc/HOW_TO_COMPILE.md) document.

### To install:

If you don't want to build JXcore by yourself, you can use pre-compiled binaries. See [Installation](doc/INSTALLATION.md) document.

### Start hacking the native core:

Macro API is available from [here](doc/native)

### To run the tests:

See [Testing](doc/TESTING.md) document.

### JXcore Contributors

Our hardworking contributors are available from [here](https://github.com/jxcore/jxcore/graphs/contributors)  
Would you like to be one of them? Fork, Hack, and send a Pull Request.


### Node.JS authors list:

List of Node.JS contributors can be found from [here](https://github.com/jxcore/jxcore/blob/master/node/AUTHORS)

** compatibility mode


# 解决jxcore嵌入到win32 GUI程序中的一些问题 #

**process.stdout**和**console.log**这些函数直接输出到stdout上，GUI程序中不存在，所以需要重定向打日志函数

- 重定向jxcore的标准输出函数
```C++
extern void log_console(const char* fmt, ...);

\#define flush_console log_console
\#define error_console log_console
\#define warn_console  error_console
```
- 导出标准输出订阅函数
```C++
typedef void(*JX_PRINT_CALLBACK)(const char* buffer);

JXCORE_EXTERN(void)
JX_AddPrintListener(JX_PRINT_CALLBACK callback);

JXCORE_EXTERN(void)
JX_RemovePrintListener(JX_PRINT_CALLBACK callback);
```
用来订阅log_console的输出

- process.stdout重定向
先定义process.subsystem
```C++
//subsystem
JS_NAME_SET(process, JS_STRING_ID("subsystem"), STD_TO_STRING("windows"));
```
在定义一个windows_wrap对象
```C++
JS_METHOD(WindowsWrap, PrintLog) {
  if (!args.IsString(0)) {
    THROW_EXCEPTION("Missing parameters (print) expects (string).");
  }

  jxcore::JXString str_key;
  args.GetString(0, &str_key);
  log_console("%s\n", *str_key);
}
JS_METHOD_END

JS_METHOD(WindowsWrap, PrintError) {
  if (!args.IsString(0)) {
    THROW_EXCEPTION("Missing parameters (print_err) expects (string).");
  }

  jxcore::JXString str_key;
  args.GetString(0, &str_key);
  if (args.GetBoolean(1))
    error_console("%s\n", *str_key);
  else
    warn_console("%s\n", *str_key);
}
JS_METHOD_END

}  // namespace node

NODE_MODULE(node_windows_wrap, node::WindowsWrap::Initialize)
```
在node_extensions.h末尾添加引用，使其启动时自动加载
```C++
NODE_EXT_LIST_ITEM(node_windows_wrap)
```

修改node.js启动脚本,增加如下内容
```javascript
// fake stdout,stderr
if( process.subsystem === 'windows' ){
  
  var Writable = NativeModule.require('stream').Writable;
  $win = process.binding('windows_wrap');
  
  util.inherits(StdoutToMem, Writable);

  function StdoutToMem(opt) {
    Writable.call(this, opt);
  }

  var fake_stdout = null, fake_stderr = null;

  fake_stdout = new StdoutToMem();
  fake_stdout.write = fake_stdout._write = function(bf) {
    $win.print(bf + '');
  };

  fake_stderr = new StdoutToMem();
  fake_stderr.write = fake_stderr._write = function(bf) {
    $win.print_err_warn(bf + '', true);
  };

  process.__defineGetter__('stdout', function() {
    return fake_stdout;
  });

  process.__defineGetter__('stderr', function() {
    return fake_stderr;
  });

  process.__defineGetter__('stdin', function() {
    console.error('stdin is not supported');
  });

  process.openStdin = function() {
    process.stdin.resume();
    return process.stdin;
  };
  return;
}
// 
```


