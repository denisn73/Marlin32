
#include "Marlin.h"

#define DISABLE_JTAG
#define MYSERIAL Serial
#define BAUDRATE 115200

void setup() {

  #ifdef DISABLE_JTAG
    afio_cfg_debug_ports(AFIO_DEBUG_SW_ONLY); // disable JTAG
  #endif

  //#if ENABLED(FILAMENT_RUNOUT_SENSOR)
  //  setup_filrunoutpin();
  //#endif

  //setup_killpin();

  //setup_powerhold();

  #if HAS_STEPPER_RESET
    disableStepperDrivers();
  #endif

  #ifdef MYSERIAL == Serial
    MYSERIAL.begin();
  #else
    MYSERIAL.begin(BAUDRATE);
  #endif
  
  //SERIAL_PROTOCOLLNPGM("start");
  //SERIAL_ECHO_START;
  
  //SERIAL_ECHOPGM(MSG_MARLIN);
  //SERIAL_CHAR(' ');
  //SERIAL_ECHOLNPGM(SHORT_BUILD_VERSION);
  //SERIAL_EOL;

  #if defined(STRING_DISTRIBUTION_DATE) && defined(STRING_CONFIG_H_AUTHOR)
    SERIAL_ECHO_START;
    SERIAL_ECHOPGM(MSG_CONFIGURATION_VER);
    SERIAL_ECHOPGM(STRING_DISTRIBUTION_DATE);
    SERIAL_ECHOLNPGM(MSG_AUTHOR STRING_CONFIG_H_AUTHOR);
    SERIAL_ECHOLNPGM("Compiled: " __DATE__);
  #endif

  //SERIAL_ECHO_START;
  //SERIAL_ECHOPAIR(MSG_FREE_MEMORY, freeMemory());
  //SERIAL_ECHOLNPAIR(MSG_PLANNER_BUFFER_BYTES, (int)sizeof(block_t)*BLOCK_BUFFER_SIZE);

  // Send "ok" after commands by default
  //for (int8_t i = 0; i < BUFSIZE; i++) send_ok[i] = true;

  // Load data from EEPROM if available (or use defaults)
  // This also updates variables in the planner, elsewhere
  //(void)settings.load();

  #if HAS_M206_COMMAND
    // Initialize current position based on home_offset
    COPY(current_position, home_offset);
  #else
    //ZERO(current_position);
  #endif

  // Vital to init stepper/planner equivalent for current_position
  //SYNC_PLAN_POSITION_KINEMATIC();

  //thermalManager.init();    // Initialize temperature loop

  #if ENABLED(USE_WATCHDOG)
    watchdog_init();
  #endif

  //stepper.init();    // Initialize stepper, this enables interrupts!
  //servo_init();

  #if HAS_PHOTOGRAPH
    OUT_WRITE(PHOTOGRAPH_PIN, LOW);
  #endif

  #if HAS_CASE_LIGHT
    case_light_on = CASE_LIGHT_DEFAULT_ON;
    case_light_brightness = CASE_LIGHT_DEFAULT_BRIGHTNESS;
    update_case_light();
  #endif

  #if ENABLED(SPINDLE_LASER_ENABLE)
    OUT_WRITE(SPINDLE_LASER_ENABLE_PIN, !SPINDLE_LASER_ENABLE_INVERT);  // init spindle to off
    #if SPINDLE_DIR_CHANGE
      OUT_WRITE(SPINDLE_DIR_PIN, SPINDLE_INVERT_DIR ? 255 : 0);  // init rotation to clockwise (M3)
    #endif
    #if ENABLED(SPINDLE_LASER_PWM)
      SET_OUTPUT(SPINDLE_LASER_PWM_PIN);
      analogWrite(SPINDLE_LASER_PWM_PIN, SPINDLE_LASER_PWM_INVERT ? 255 : 0);  // set to lowest speed
    #endif
  #endif

  #if HAS_BED_PROBE
    endstops.enable_z_probe(false);
  #endif

  #if ENABLED(USE_CONTROLLER_FAN)
    SET_OUTPUT(CONTROLLER_FAN_PIN); //Set pin used for driver cooling fan
  #endif

  #if HAS_STEPPER_RESET
    enableStepperDrivers();
  #endif

  #if ENABLED(DIGIPOT_I2C)
    digipot_i2c_init();
  #endif

  #if ENABLED(DAC_STEPPER_CURRENT)
    dac_init();
  #endif

  #if (ENABLED(Z_PROBE_SLED) || ENABLED(SOLENOID_PROBE)) && HAS_SOLENOID_1
    OUT_WRITE(SOL1_PIN, LOW); // turn it off
  #endif

  //setup_homepin();

  #if PIN_EXISTS(STAT_LED_RED)
    OUT_WRITE(STAT_LED_RED_PIN, LOW); // turn it off
  #endif

  #if PIN_EXISTS(STAT_LED_BLUE)
    OUT_WRITE(STAT_LED_BLUE_PIN, LOW); // turn it off
  #endif

  #if ENABLED(RGB_LED) || ENABLED(RGBW_LED)
    SET_OUTPUT(RGB_LED_R_PIN);
    SET_OUTPUT(RGB_LED_G_PIN);
    SET_OUTPUT(RGB_LED_B_PIN);
    #if ENABLED(RGBW_LED)
      SET_OUTPUT(RGB_LED_W_PIN);
    #endif
  #endif

  //lcd_init();
  
  #if ENABLED(SHOW_BOOTSCREEN)
    #if ENABLED(DOGLCD)
      safe_delay(BOOTSCREEN_TIMEOUT);
    #elif ENABLED(ULTRA_LCD)
      bootscreen();
      #if DISABLED(SDSUPPORT)
        lcd_init();
      #endif
    #endif
  #endif

  #if ENABLED(MIXING_EXTRUDER) && MIXING_VIRTUAL_TOOLS > 1
    // Initialize mixing to 100% color 1
    for (uint8_t i = 0; i < MIXING_STEPPERS; i++)
      mixing_factor[i] = (i == 0) ? 1.0 : 0.0;
    for (uint8_t t = 0; t < MIXING_VIRTUAL_TOOLS; t++)
      for (uint8_t i = 0; i < MIXING_STEPPERS; i++)
        mixing_virtual_tool_mix[t][i] = mixing_factor[i];
  #endif

  #if ENABLED(BLTOUCH)
    // Make sure any BLTouch error condition is cleared
    bltouch_command(BLTOUCH_RESET);
    set_bltouch_deployed(true);
    set_bltouch_deployed(false);
  #endif

  #if ENABLED(EXPERIMENTAL_I2CBUS) && I2C_SLAVE_ADDRESS > 0
    i2c.onReceive(i2c_on_receive);
    i2c.onRequest(i2c_on_request);
  #endif

  #if ENABLED(ENDSTOP_INTERRUPTS_FEATURE)
    setup_endstop_interrupts();
  #endif
  
  #if ENABLED(SWITCHING_EXTRUDER)
    move_extruder_servo(0);  // Initialize extruder servo
  #endif

  #if ENABLED(SWITCHING_NOZZLE)
    move_nozzle_servo(0);  // Initialize nozzle servo
  #endif
  
}

void loop() {
  
}

