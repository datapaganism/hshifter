Import("env")

board_config = env.BoardConfig()
board_config.update("build.hwids", [
  ["0x1209", "0xA137"]
])
board_config.update("build.usb_product", "H-Shifter")
board_config.update("vendor", "Aleno")