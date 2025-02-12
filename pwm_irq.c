#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SERVO_PIN 22 // Utilização no simulador
// #define SERVO_PIN 12 // Utilização na placa
#define PWM_FREQ 50.0f

void config_servo_pwm() {
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);
    pwm_config config = pwm_get_default_config();
    float clkdiv = 125000000.0f / (PWM_FREQ * 65535.0f);
    pwm_config_set_clkdiv(&config, clkdiv);
    pwm_config_set_wrap(&config, 65535);
    pwm_init(slice_num, &config, true);
}

void set_servo_us(uint us) {
    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);
    uint16_t level = (us / 20000.0f) * 65535;
    pwm_set_chan_level(slice_num, PWM_CHAN_A, level);
}

void smooth_movement(uint start_us, uint end_us) {
    int step = (start_us < end_us) ? 5 : -5;
    for (uint us = start_us; (step > 0) ? (us <= end_us) : (us >= end_us); us += step) {
        set_servo_us(us);
        sleep_ms(10);
    }
}

int main() {
    stdio_init_all();
    config_servo_pwm();

    // Movimentos iniciais
    set_servo_us(2400);  // 180 graus
    sleep_ms(5000);
    set_servo_us(1470);  // 90 graus
    sleep_ms(5000);
    set_servo_us(500);   // 0 graus
    sleep_ms(5000);

    // Movimento suave contínuo
    while (1) {
        smooth_movement(500, 2400);   // 0 -> 180 graus
        smooth_movement(2400, 500);   // 180 -> 0 graus
    }
}
