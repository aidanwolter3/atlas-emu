#ifndef INPUT_INPUT_H_
#define INPUT_INPUT_H_

class Input {
 public:
  virtual ~Input() {}

  struct State {
    bool a, b;
    bool select, start;
    bool up, down, left, right;
  };
  virtual State GetButtonState() = 0;
};

class FakeInput : public Input {
 public:
  // Input implementation:
  Input::State GetButtonState() override {
    return {.a = false,
            .b = false,
            .select = false,
            .start = false,
            .up = false,
            .down = false,
            .left = false,
            .right = false};
  }
};

#endif  // INPUT_INPUT_H_
