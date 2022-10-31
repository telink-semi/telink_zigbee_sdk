1) fix flash write issue(need to valid the data wroten to the flash to avoid
writing the error data to the falsh)
2) discard the current packet when rf is busy to avoid the error data
3) the buffer will be overfolwn once the group table is larger than 15
4) the link key might be wrong after OTA
5) the data might can't be sent out for the sleep device
6) add voltage detection for zbit flash
