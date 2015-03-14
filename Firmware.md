## HiDeO firmware ##
### About ###
#### HiDeO license ####
#### AVRLIB license ####

### How to build and install ###
#### Requirements ####
  * [GCC](http://gcc.gnu.org) for AVR (generally called avr-gcc)
  * [AVR Libc - Standard C library](http://www.nongnu.org/avr-libc) for AVR (generally called avr-libc)
  * [AVRDUDE - AVR Downloader/UploaDEr](http://www.nongnu.org/avrdude) or another software uploader (ie. [UISP](http://www.nongnu.org/uisp))

#### Compilation ####
HiDeO firmware now use autotools.

##### Stable version #####
Place you into HiDeO firmware directory and simply do:
```
./configure --host=avr
make
```
##### Development version #####
```
./autogen.sh
./configure --host=avr
make
```

#### Upload ####
Using STK500 ISP on /dev/ttyUSB0:
```
avrdude -p m32 -c stk500v2 -P /dev/ttyUSB0 -u -U flash:w:src/hideo.hex:i
```

Using STK200 on parallel port:
```
avrdude -p m32 -c stk200 -u -U flash:w:src/hideo.hex:i
```

### Documentation ###
#### Operating System ####
  * How to write a new application for HiDeO's OS
Since trunk at 30th revision, we provide an application template.

  * Copy files
```
cp app_template.c app_myapp.c
cp app_template.h app_myapp.h
```

  * Find & replace
In app\_myapp.c and app\_myapp.h, change all occurrences 'app\_template' by 'app\_myapp' and, in app\_myapp.h, replace 'APP\_TEMPLATE' by 'APP\_MYAPP'.

  * Add your application in main.c, in example:
```
int main (void)
{
	cli();

	// eventmanager_init() must be called before all modules that use events...
	eventmanager_init();

//	app_keyboard_init();
//	app_temperature_init();
	app_motor_init();
	app_mouse_init();
	app_myapp_init(); // <-- Put your application here


	app_mainmenu_init();
...
```

  * Put your application in the mainmenu:
In app\_mainmenu.h, change definition
```
#define app_mainmenu_init()		menu_t mainmenu;					\
	menu_item_t mainmenu_items[] = {							\
		{ PSTR("Date"), NULL },								\
		{ PSTR("Motor"), &app_motor },							\
		{ PSTR("Mouse"), &app_mouse },							\
		{ PSTR("My app"), &app_myapp },							\
	};											\
	mainmenu.menu_items = mainmenu_items;							\
	mainmenu.item_count = sizeof(mainmenu_items)/sizeof(menu_item_t);			\
	application_t app_mainmenu = { &_app_mainmenu_init, &_app_mainmenu_event_handler, &mainmenu }
```