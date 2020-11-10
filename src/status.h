#ifndef STATUS_H_
#define STATUS_H_

struct StatusRegister {
  bool carry = false;
  bool zero = false;
  bool int_disable = false;
  bool bcd_mode = false;
  bool brk = false;
  bool overflow = false;
  bool sign = false;
};

#endif  // STATUS_H_
