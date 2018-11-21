template <class E>
class BinarySet {
  int _flags;
  public:
  BinarySet() {
    _flags = 0;
  }
  BinarySet(int flags) { _flags = flags; }

  int setFlag(E flag) {
    _flags|=flag;
    return _flags;
  };
  int resetFlag(E flag) {
    _flags&=(flag^0xFF); // Reverse map and remove the set flags
    return _flags;
  };
  int getFlags() {
    return _flags;
  };
};


