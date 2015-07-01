# **EspLua.ru** #
Версия 1.1.2

Проект в стадии разработки.
На текущий момент содержит версии только для тестов.

###Lua для Wi-Fi модулей esp8266
Построен на частях от [ESP8266 sdk 1.1.2 + Патчи](http://bbs.espressif.com/viewforum.php?f=5) с изменениями<br />
Ядро Lua на основе [eLua проекта](http://www.eluaproject.net/)<br />
Сjson на основе [lua-cjson](https://github.com/mpx/lua-cjson)<br />
Файловая система основана на [spiffs](https://github.com/pellepl/spiffs)<br />

# Резюме
- Для детей, поиграться на вечер 
- Легкий доступ к беспроводному маршрутизатору
- Основа (ядро) на Lua 5.1.4
- Предпочтительно программирование событий-Драйв.
- Встроено json, file, timer, pwm, i2c, spi, 1-wire, net, mqtt, coap, gpio, wifi, adc, uart and system api.
- Используется индекс для доступа к gpio, i2c, pwm.
- Используются Integer и Float.

# Журнал версий

2015-07-01 - новые команды wifi.sta.rssi() и wifi.max_tpw()<br />
2015-06-27 - Исправления/дополнения + новая команда file.fsstat()<br />
2015-06-25 - Начало проекта<br />

#Стартовый лог

```lua
EspLua.ru 1.1.2 build 20150701  powered by Lua 5.1.4
lua: cannot open init.lua
> =node.heap()
20168
> = wifi.sta.rssi()
-28
```



##Таблица GPIO

<a id="gpio_map"></a>
<table>
  <tr>
    <th scope="col">IO index</th><th scope="col">ESP8266 pin</th><th scope="col">IO index</th><th scope="col">ESP8266 pin</th>
  </tr>
  <tr>
    <td>0 [*]</td><td>GPIO16</td><td>8</td><td>GPIO15</td>
  </tr>
  <tr>
    <td>1</td><td>GPIO5</td><td>9</td><td>GPIO3</td>
   </tr>
   <tr>
    <td>2</td><td>GPIO4</td><td>10</td><td>GPIO1</td>
  </tr>
  <tr>
    <td>3</td><td>GPIO0</td><td>11</td><td>GPIO9</td>
   </tr>
   <tr>
    <td>4</td><td>GPIO2</td><td>12</td><td>GPIO10</td>
  </tr>
  <tr>
    <td>5</td><td>GPIO14</td><td></td><td></td>
   </tr>
   <tr>
    <td>6</td><td>GPIO12</td><td></td><td></td>
  </tr>
  <tr>
    <td>7</td><td>GPIO13</td><td></td><td></td>
   </tr>
</table>
#### [*] В текущей версии D0(GPIO16) может быть использован только в качестве ножки ввода для чтения уровня 1 или 0. Другие функции, пока, с ним не работают.

#Прошивка Flash 
0x00000.bin: 0x00000<br />
0x0C000.bin: 0x0C000<br />
esp_init_data_default.bin: 0x7C000<br /> 
blank.bin: 0x7E000<br />

#Cкорость передачи RS-232 TTL по умолчанию
baudrate:9600

#Начните игру

####Подключение к точке доступа

```lua
    ip = wifi.sta.getip()
    print(ip)
    --nil
    wifi.setmode(wifi.STATION)
    wifi.sta.config("SSID","password")
    ip = wifi.sta.getip()
    print(ip)
    --192.168.18.110
```

####Манипулирование оборудованием, как в Arduino

```lua
    pin = 1
    gpio.mode(pin,gpio.OUTPUT)
    gpio.write(pin,gpio.HIGH)
    print(gpio.read(pin))
```

####Простейший клиент HTTP в стиле nodejs

```lua
    -- A simple http client
    conn=net.createConnection(net.TCP, 0)
    conn:on("receive", function(conn, payload) print(payload) end )
    conn:connect(80,"115.239.210.27")
    conn:send("GET / HTTP/1.1\r\nHost: www.baidu.com\r\n"
        .."Connection: keep-alive\r\nAccept: */*\r\n\r\n")
```

####Или простейший HTTP-сервер

```lua
    -- A simple http server
    srv=net.createServer(net.TCP)
    srv:listen(80,function(conn)
      conn:on("receive",function(conn,payload)
        print(payload)
        conn:send("<h1> Hello, NodeMcu.</h1>")
      end)
      conn:on("sent",function(conn) conn:close() end)
    end)
```

####Подключение к MQTT Брокеру

```lua
-- init mqtt client with keepalive timer 120sec
m = mqtt.Client("clientid", 120, "user", "password")

-- setup Last Will and Testament (optional)
-- Broker will publish a message with qos = 0, retain = 0, data = "offline"
-- to topic "/lwt" if client don't send keepalive packet
m:lwt("/lwt", "offline", 0, 0)

m:on("connect", function(con) print ("connected") end)
m:on("offline", function(con) print ("offline") end)

-- on publish message receive event
m:on("message", function(conn, topic, data)
  print(topic .. ":" )
  if data ~= nil then
    print(data)
  end
end)

-- for secure: m:connect("192.168.11.118", 1880, 1)
m:connect("192.168.11.118", 1880, 0, function(conn) print("connected") end)

-- subscribe topic with qos = 0
m:subscribe("/topic",0, function(conn) print("subscribe success") end)
-- or subscribe multiple topic (topic/0, qos = 0; topic/1, qos = 1; topic2 , qos = 2)
-- m:subscribe({["topic/0"]=0,["topic/1"]=1,topic2=2}, function(conn) print("subscribe success") end)
-- publish a message with data = hello, QoS = 0, retain = 0
m:publish("/topic","hello",0,0, function(conn) print("sent") end)

m:close();
-- you can call m:connect again

```

#### UDP клиент и сервер
```lua
-- a udp server
s=net.createServer(net.UDP)
s:on("receive",function(s,c) print(c) end)
s:listen(5683)

-- a udp client
cu=net.createConnection(net.UDP)
cu:on("receive",function(cu,c) print(c) end)
cu:connect(5683,"192.168.18.101")
cu:send("hello")
```

####Сделайть что-нибудь с ШИМ 
```lua
  function led(r,g,b)
    pwm.setduty(1,r)
    pwm.setduty(2,g)
    pwm.setduty(3,b)
  end
  pwm.setup(1,500,512)
  pwm.setup(2,500,512)
  pwm.setup(3,500,512)
  pwm.start(1)
  pwm.start(2)
  pwm.start(3)
  led(512,0,0) -- red
  led(0,0,512) -- blue
```

####Мигать светодиодом
```lua
  lighton=0
  tmr.alarm(1,1000,1,function()
    if lighton==0 then
      lighton=1
      led(512,512,512)
    else
      lighton=0
      led(0,0,0)
    end
  end)
```

####Запустить что-то, при старте системы (авто-запускаемый файл при старте)
```lua
  --init.lua will be excuted
  file.open("init.lua","w")
  file.writeline([[print("Hello, do this at the beginning.")]])
  file.close()
  node.restart()  -- this will restart the module.
```

####Пародия на Telnet
```lua
    -- a simple telnet server
    s=net.createServer(net.TCP,180)
    s:listen(2323,function(c)
       function s_output(str)
          if(c~=nil)
             then c:send(str)
          end
       end
       node.output(s_output, 0)   -- re-direct output to function s_ouput.
       c:on("receive",function(c,l)
          node.input(l)           -- works like pcall(loadstring(l)) but support multiple separate line
       end)
       c:on("disconnection",function(c)
          node.output(nil)        -- un-regist the redirect output function, output goes to serial
       end)
       print("Welcome to NodeMcu world.")
    end)
```

####Использование DS18B20
```lua
    -- read temperature with DS18B20
    node.compile("ds18b20.lua")   --  run this only once to compile and save to "ds18b20.lc"
    t=require("ds18b20")
    t.setup(9)
    addrs=t.addrs()
    -- Total DS18B20 numbers, assume it is 2
    print(table.getn(addrs))
    -- The first DS18B20
    print(t.read(addrs[1],t.C))
    print(t.read(addrs[1],t.F))
    print(t.read(addrs[1],t.K))
    -- The second DS18B20
    print(t.read(addrs[2],t.C))
    print(t.read(addrs[2],t.F))
    print(t.read(addrs[2],t.K))
    -- Just read
    print(t.read())
    -- Just read as centigrade
    print(t.read(nil,t.C))
    -- Don't forget to release it after use
    t = nil
	ds18b20 = nil
    package.loaded["ds18b20"]=nil
```

####Работа с дисплеем через I2C с u8glib
u8glib это графическая библиотека с поддержкой различных дисплеев. 
Интеграция в nodemcu разработана на SSD1306 основе дисплея подключенного через порт I2C. 
Другие виды отображения и подключения SPI будет добавлены по "просьбе трудящихся".


#####Соединение I2C 
Подключите SDA и SCL на любые свободные GPIOs. Пример `lua_examples/u8glib/graphics_test.lua` SDA=5 (GPIO14) и SCL=6 (GPIO12). Они используются для настройки драйвера I2C EspLua.ru перед доступом к дисплею:
```lua
sda = 5
scl = 6
i2c.setup(0, sda, scl, i2c.SLOW)
```

#####Соединение SPI
Используется аппартный HSPI на конкретных выводах ESP8266:
* HSPI CLK  = GPIO14
* HSPI MOSI = GPIO13
* HSPI MISO = GPIO12 (not used)

Другие выводы на любых доступных GPIO:
* CS
* D/C
* RES (опция для некоторых дисплеев)

Также обратитесь к последовательности инициализации, например в `lua_examples/u8glib/graphics_test.lua`:
```lua
spi.setup(1, spi.MASTER, spi.CPOL_LOW, spi.CPHA_LOW, spi.DATABITS_8, 0)
```

Комментарии и остальное на форуме  [esp8266.ru](http://esp8266.ru/forum/)<br />