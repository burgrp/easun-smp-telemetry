class EspError : public exception {
public:
  esp_err_t code;
  EspError(esp_err_t code) : code(code) {}

  virtual const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW {
    return esp_err_to_name(code);
  }
};

#define ESP_THROW(x)                                                           \
  do {                                                                         \
    esp_err_t err_rc_ = (x);                                                   \
    if (unlikely(err_rc_ != ESP_OK)) {                                         \
      throw new EspError(err_rc_);                                             \
    }                                                                          \
  } while (0)


