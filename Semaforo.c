#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define red_led 1 
#define yellow_led 5 
#define green_led 9 
#define button_pin 18
#define pedestre_pin 28 

#define buzzer_pin 13
#define buzzer_frequency 100

void pwm_init_buzzer(uint pin){
  gpio_set_function(pin, GPIO_FUNC_PWM);
  uint slice_num = pwm_gpio_to_slice_num(pin);

  //configuração com a frequencia escolhida
  pwm_config config = pwm_get_default_config();
  pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (buzzer_frequency * 4096)); // divisor de clock
  pwm_init(slice_num, &config, true);

  pwm_set_gpio_level(pin, 0);
}

void beep(uint pin, uint duration_ms) {
    // Obter o slice do PWM associado ao pino
    uint slice_num = pwm_gpio_to_slice_num(pin);

    // Configurar o duty cycle para 50% (ativo)
    pwm_set_gpio_level(pin, 2048);

    // Temporização
    sleep_ms(duration_ms);

    // Desativar o sinal PWM (duty cycle 0)
    pwm_set_gpio_level(pin, 0);

    // Pausa entre os beeps
    sleep_ms(100); // Pausa de 100ms
}

void leds_ativos(bool a, bool b, bool c,bool d){
  gpio_put(red_led, a);
  gpio_put(green_led, b); 
  gpio_put(yellow_led, c);
  gpio_put(pedestre_pin, d);
}

//função que é chamada caso o usuário aperte o botão
void ordem_botao(){
  leds_ativos(false,false,true,false); //amarelo
  sleep_ms(5000); //5 segundos do led amarelo
  leds_ativos(true,false,false,true); //vermelho
  beep(buzzer_pin, 15000); // 15s de bipe 
}


int main() {
  stdio_init_all();

  // configurando todos os LEDs 
  gpio_init(red_led);
  gpio_init(yellow_led);
  gpio_init(green_led);
  gpio_init(pedestre_pin);

  gpio_set_dir(red_led, GPIO_OUT);
  gpio_set_dir(green_led, GPIO_OUT);
  gpio_set_dir(yellow_led, GPIO_OUT);
  gpio_set_dir(pedestre_pin, GPIO_OUT);

  //configurando o botão
  gpio_init(button_pin);
  gpio_set_dir(button_pin, GPIO_IN);
  gpio_pull_up(button_pin);

  //inicializa o buzzer
  pwm_init_buzzer(buzzer_pin);
  
  while (true){
    int i;
    bool botao_apertado = false;

    leds_ativos(false,true,false,false); //verde

    //verifica se o botão foi apertado a cada 10ms, porque a cada 100ms as vezes não contabilizava 
    for (i=0; i< 800;i++){  
      if(gpio_get(button_pin) == 0){
        printf("botao apertado.\n");
        ordem_botao();
        botao_apertado = true;
        break;
        }
      sleep_ms(10); 
    }

    if (botao_apertado){  //if para reiniciar o laço de repetição caso o botao seja apertado
      continue;
    } 

    
    leds_ativos(false,false,true,false); //amarelo
    sleep_ms(2000); //não verifica se o botao foi acionado por já estar no amarelo

    leds_ativos(true,false,false,true); //vermelho
    sleep_ms(10000);
    //não tem verificação do botão porque já está no sinal vermelho
    
}}
