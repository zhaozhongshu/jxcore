// Copyright & License details are available under JXCORE_LICENSE file

#ifndef SRC_WRAPPERS_JXUTILS_WRAP_H_
#define SRC_WRAPPERS_JXUTILS_WRAP_H_

#include "jx/Proxy/JSEngine.h"
#include "jx/commons.h"

namespace node {

class WindowsWrap {
 private:
  static DEFINE_JS_METHOD(PrintLog);

  static DEFINE_JS_METHOD(PrintError);

 public:
  INIT_CLASS_MEMBERS() {
    SET_CLASS_METHOD("print", PrintLog, 1);
    SET_CLASS_METHOD("print_err_warn", PrintError, 1);
  }
  END_INIT_MEMBERS
};
}  // namespace node

#endif  // SRC_WRAPPERS_JXUTILS_WRAP_H_
