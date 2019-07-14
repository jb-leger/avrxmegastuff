
class PinDesc:
    def __init__(self, name):
        assert name[0] in ("A", "B", "C", "D", "E", "F")
        assert name[1] in ("0", "1", "2", "3", "4", "5", "6", "7")
        self._name = name

    @property
    def port(self):
        return f"PORT{self._name[0]}"

    @property
    def pin(self):
        return self._name[1]

    @property
    def pin_bm(self):
        return f"PIN{self._name[1]}_bm"

    @property
    def pin_ctrl_name(self):
        return f"PIN{self._name[1]}CTRL"

    @property
    def pinctrl(self):
        return f"{self.port}.{self.pin_ctrl_name}"
