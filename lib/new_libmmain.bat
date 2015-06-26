del libmmain.a
md main.lib
cd main.lib
C:\Espressif\xtensa-lx106-elf\bin\xtensa-lx106-elf-ar x C:\Espressif\ESP8266_SDK\lib\libmain.a
@rem 
@rem del: spi_flash.o spi_overlap.o ets_timer.o
C:\Espressif\xtensa-lx106-elf\bin\xtensa-lx106-elf-ar ru ..\libmmain.a vector.o kernel-vector.o nmi-vector.o debug-vector.o double-vector.o user-vector.o ..\0\48app_main.o eagle_lib.o eagle_lwip_if.o user_interface.o mem_manager.o
cd ..
rd /q /s main.lib
