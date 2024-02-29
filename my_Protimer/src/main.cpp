// created by gaurav on 29/02/2024
// completed | checked 3/11/23
#include <main.h>
#include <lcd.h>

static void protimer_event_dispatcher(protimer_t *const mobj, event_t const *const e);
static uint8_t process_button_pad_value(uint8_t btn_pad_value);
static void display_init(void);

static protimer_t protimer;
void setup()
{
  Serial.begin(115200);
  display_init();
  Serial.println("productive timer application");
  Serial.println("==================");
  lcd_clear();
  lcd_set_cursor(0,0);
  lcd_print_string("Productive Time");
  lcd_set_cursor(4,1);
  lcd_print_string("Tracker");
  delay(1500);
  lcd_clear();
  lcd_set_cursor(2,0);
  lcd_print_string("Designed By");
  lcd_set_cursor(4,1);
  lcd_print_string("Gaurav");
  delay(1500);
  lcd_clear();
  pinMode(PIN_BUTTON1, INPUT);
  pinMode(PIN_BUTTON2, INPUT);
  pinMode(PIN_BUTTON3, INPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  protimer_init(&protimer);
}

void loop()
{
  uint8_t b1,b2,b3,btn_pad_value;
  protimer_user_event_t ue;
  static uint32_t current_time = millis();
  static protimer_tick_event te;

  b1=digitalRead(PIN_BUTTON1);  // reading button 1
  b2=digitalRead(PIN_BUTTON2);  // reading button 2
  b3=digitalRead(PIN_BUTTON3);  // reading button 3

  btn_pad_value = (b1<<2) | (b2<<1) | b3;   // preparing a value from button input

  btn_pad_value = process_button_pad_value(btn_pad_value);  // check if button is bounced, pressed, not_pressed?

  if (btn_pad_value)    // if button is pressed then only enters
  {                     // four conditions cuz: 3buttons + 1combination button press
    if (btn_pad_value == BTN_PAD_VALUE_INC_TIME)
    {
      ue.super.sig = INC_TIME;
    }
    else if (btn_pad_value == BTN_PAD_VALUE_DEC_TIME)
    {
      ue.super.sig = DEC_TIME;
    }
    else if (btn_pad_value == BTN_PAD_VALUE_SP)
    {
      ue.super.sig = START_PAUSE;
    }
    else if (btn_pad_value == BTN_PAD_VALUE_ABRT)
    {
      ue.super.sig = ABRT;
    }
    protimer_event_dispatcher(&protimer, &ue.super);
  }

  if(millis() - current_time >= 100)
  {
    current_time = millis();
    te.super.sig = TIME_TICK;
    if(++te.ss > 10) te.ss = 1;
    {
      protimer_event_dispatcher(&protimer, &te.super);
    }
  }
}


static void protimer_event_dispatcher(protimer_t *const mobj, event_t const *const e)
{
  event_status_t status;
  protimer_state_t source, target;

  source = mobj->active_state;
  status = protimer_state_machine(mobj, e);
  
  if (status == EVENT_TRANSITION)
  {
    target = mobj->active_state;
    event_t ee;

    // EXIT 
    ee.sig = EXIT;
    mobj->active_state = source;
    protimer_state_machine(mobj,&ee);

    // ENTRY
    ee.sig = ENTRY;
    mobj->active_state = target;
    protimer_state_machine(mobj,&ee);
  }
}

/*
static uint8_t process_button_pad_value(uint8_t btn_pad_value)
This function takes the button input values process them and return value is eigther |bounced|presses|not_pressed
To check if button is pressed and remove the deboucing effect, it compaire the button pressed and its timming using
the millis() function compaire with 50.
Return statement is only on case BOUNCED
If the button is pressed then the btn_pad_value is sent forward for processing,
otherwise if button is bounced or not_pressed then 0 is returned.
*/
static uint8_t process_button_pad_value(uint8_t btn_pad_value)
{
  static button_state_t btn_sm_state = NOT_PRESSED;
  static uint32_t curr_time = millis();   // take time from uC is started into current time regis
  switch (btn_sm_state)
  {
    case NOT_PRESSED:
    {
      if (btn_pad_value)
      {
        btn_sm_state = BOUNCED;
        curr_time = millis();
      }
      break;      
    }
    case BOUNCED:
    {
      if (millis() - curr_time >= 50)
      {
        if (btn_pad_value)
        {
          btn_sm_state = PRESSED;
          return btn_pad_value;   // only returned when the button is found pressed.
        }
        else
        {
          btn_sm_state = NOT_PRESSED;
        }
      }
      break;
    }
    case PRESSED:
    {
      if (!btn_pad_value)
      {
        btn_sm_state = BOUNCED;
        curr_time = millis();
      }
      break;
    }
    
    default:
      break;
  }
  return 0;
}

static void display_init(void)
{
  lcd_begin(16, 2);
  lcd_clear();
  lcd_move_cursor_L_to_R();
  lcd_set_cursor(0, 0);
  lcd_no_auto_scroll();
  lcd_cursor_off();
}