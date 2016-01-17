// Copyright & License details are available under JXCORE_LICENSE file

#include "windows_wrap.h"
#include "jx/extend.h"
#include <climits>
#if defined(_MSC_VER)
#include <windows.h>
#else
#include <unistd.h>
#define Sleep(x) usleep((x)*1000)
#endif

namespace node {

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