## V3.7.1.2_Patch_0001(PR)

### NOTE
* Scope of versions involved
  * V3.7.1.0
  * V3.7.1.1
  * V3.7.1.2

### Bug Fixes
* **calibration**
  * (B91/B92/TL721X/TL321X) Modified the logic for determining frequency_offset_value in the drv_calib_freq_offset interface to resolve the issue where the frequency offset calibration value for the chip fixture was not taking effect.

### BREAKING CHANGES
* **calibration**
  * (B91/B92/TL721X/TL321X) Modified the internal logic for determining frequency_offset_value in the drv_calib_freq_offset interface to align with the write logic of the chip fixture's frequency offset calibration value. This ensures proper utilization of the RF frequency offset calibration value stored in the flash, preventing potential RF frequency offset issues. This fix impacts all SDKs and applications that require RF frequency offset calibration.


### Bug Fixes
* **calibration**
  * (B91/B92/TL721X/TL321X) 修改了drv_calib_freq_offset接口中判断frequency_offset_value的逻辑，以解决芯片夹具频偏校准值不生效的问题。

### BREAKING CHANGES
* **calibration**
  * (B91/B92/TL721X/TL321X)修改了drv_calib_freq_offset接口中判断frequency_offset_value的内部逻辑，使其保证和芯片夹具频偏校准值写入逻辑一致。保证flash中的RF频偏校准值被正确使用，避免产生RF频偏问题。此问题影响所有需要进行RF频偏校准的SDK和应用。
