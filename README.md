# Контроллер для гирлянды типа "Роса" на базе Arduino Nano
Не так давно я обзавелся гирлядой "Роса". Решил написть для нее контроллер на базе Arduino Nano с использованием кнопки и пульта ДУ. Гирлянда состоит из 6 линий Росы. Управляется с помощью кнопки и пульта ДУ. Программа имеет 6 эффектов. Каждый эффект сохраняется в EPROM память Arduino. 
## Список эффектов 
1. Плавное одновременное включение и выключение
2. Постоянное горение
3. Поочередное включение и выключение каждой линиии
4. Все включаются и гаснут по кругу
5. Мигание попарно
6. Поочередное включение и выключение каждой линиии только плавно
## Что интересного?
* Работает от обычного 5V зарядника
* Управление осуществляется через кнопку или пульт ДУ
* Последний выбранный режим сохраняется в память и воспроизводится после подачи питания
* Помещается в распределительную коробку 50х50х20
## Что плохо
* Максимум 6 линий ибо, ATmega328 имеет только 6 ШИМ портов :(
* Мало эффектов, однако в будущем может быть что-то добавлю
* Почти нечитаемый говно-код
## Схема подключения
* Роса - D3, D5, D6, D9, D10, D11
* Кнопка - D8
* Приемник (VS1838B) - D2
* Разъем 5.5х2.5 - VIN

![Untitled Sketch_bb](https://github.com/parzival-2077/controller_for_rosa/assets/89209738/73f65c92-7630-4a1e-9a35-bfc6b7978b9d)
## Ссылки на компоненты
* [Arduino Nano](https://clck.ru/37X6id)
* Роса [одна](https://clck.ru/37X6v5) и [разные](https://clck.ru/37X72Z)
* Кнопку, разъем и пульт с примеником можно купить в любом магазине радиоэлектроники
