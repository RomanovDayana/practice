// practice.cpp: главный файл проекта.

#include "stdafx.h"             
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <malloc.h>
#include <clocale>


using namespace System;
using namespace std;
using namespace System::IO;

//объявление символических констант,обозначение кнопок 
#define ENTER 13
#define ESC 27
#define UP 72
#define DOWN 80
#define HOME 71
#define END 79


const char dan[7][56] = {
  "Какой самый дорогой предмет одежды ?                   ",
  "Какой цвет самый популярный?                           ",
  "Инвертированный и алфавитный список поставщиков        ",
  "Когда был поставлен самый первый предмет одежды?       ",
  "Диаграмма                                              ",
  "Есть ли одинаковая цена на разные предметы одежды?     ",
  "Выход                                                  "
};

const char BlankLine[] = "                                                        "; //56 пробелов
//структура исходных данных
struct z {
  char name[20]; //фио поставвщика
  char vid[20]; //наименование товара
  char color[20]; //цвет товара
  char size[20]; //размер товара
  long kolvo; //количество товара
  char data[11]; //дата поставки
  long firstsumma; //оптовая цена
  long pastsumma; // розничная цена
};
//глобальное описание структуры двустроннего списка
struct sp {
  char fio[20];
  long Pastsumma;
  struct sp * sled;
  struct sp * pred;
};
// описание структуры алфавитного списка цветов
struct Zvet {
  char Сolor[20];
  long kolvo;
  struct Zvet * sled_;
};
//указание  используемых функций 
int menu(int);      
void maximpastsum(struct z *, int NC);
void text_data(char * , char * ,int NC);
void Alfalist(struct z * ,int NC, struct sp * spisok);
void vstavka(struct z * , char * , struct sp **, int NC);
void first(struct z * , int NC);
void diagram(struct z * , int NC, struct sp * spisok);
void zvet(struct z * , int NC, struct Zvet ** );
void vstavka2(struct z * , char * , int NC, struct Zvet ** spisok2);
void dop(struct z * , int NC);
//на локальном уровне осуществляем открытие файла,считываем данные из файла в выделенную память 
int main() 
{
  int i, n, NC;
  FILE * in;
  struct z * provider; //указать на структуру
  setlocale(LC_CTYPE, "Russian"); //руссификация
  Console::CursorVisible::set(false); //убрать курсор
  Console::BufferHeight = Console::WindowHeight; //установка размера буфера по ширине
  Console::BufferWidth = Console::WindowWidth; //установка размера буфера по высоте
  if ((in = fopen("курсовая.txt", "r")) == NULL) //попытка открыть файл
	{
    printf("\nФайл курсовая.txt не открыт !");//при неудачном открытии выводим соообщение “Файл не открыт!”
    getch();
    exit(1);
	}
  fscanf(in, "%d", & NC); //прочитали файл
  provider = (struct z * ) malloc(NC * sizeof(struct z)); // выделяем память
  for (i = 0; i < NC; i++) //считываем данные из списка
    fscanf(in, "%s%s%s%s%ld%s%ld%ld", provider[i].name, provider[i].vid, provider[i].color, provider[i].size, & provider[i].kolvo, provider[i].data, & provider[i].firstsumma, & provider[i].pastsumma);
  
  struct sp *spisok = NULL;
  for (i = 0; i < NC; i++)
	vstavka(provider, provider[i].name, &spisok, NC);	//вставляем из списка названия имен и красим символы в чѐрный цвет

  struct Zvet *spisok2 = NULL;
  for (i = 0; i < NC; i++)
    vstavka2(provider, provider[i].color, NC, &spisok2);

  Console::ForegroundColor = ConsoleColor::White;
  Console::BackgroundColor = ConsoleColor::DarkRed;
  Console::Clear();
  Console::ForegroundColor = ConsoleColor::White;
  Console::BackgroundColor = ConsoleColor::DarkRed;
  for (i = 0; i < NC; i++)//выводим данные из списка на экран 
    printf("\n  %-16s %-16s %-12s %-7s %-6ld %-16s %5ld %7ld", provider[i].name, provider[i].vid, provider[i].color, provider[i].size, provider[i].kolvo, provider[i].data, provider[i].firstsumma, provider[i].pastsumma);
  getch();
 //в цикле создаем область для вопросов и окрашиваем ее в цвета,устанавливаем для букв цвет и подсветку для выбранного вопроса
  while (1)
  {
    Console::ForegroundColor = ConsoleColor::Gray;
    Console::BackgroundColor = ConsoleColor::DarkRed;
    Console::Clear();
    Console::ForegroundColor = ConsoleColor::White;
    Console::BackgroundColor = ConsoleColor::Black;
    Console::CursorLeft = 10;//первая точка, откуда будем закрашивать область меню
    Console::CursorTop = 4;
    printf(BlankLine);

    for (i = 0; i < 7; i++) //размещение вопросов 
		{
      Console::CursorLeft = 10;
      Console::CursorTop = i + 5;
      printf(" %s", dan[i]);
		}

    Console::CursorLeft = 10;//последняя точка,где будет заканчиваться выделенная область под меню
    Console::CursorTop = 12;
    printf(BlankLine);
    n = menu(7);//выбор вопроса в меню 
    switch (n) 
	{
    case 1:
      maximpastsum(provider, NC);
      break;
    case 2:
      zvet(provider, NC, &spisok2);
      break;
    case 3:
      Alfalist(provider, NC, spisok);
      break;
    case 4:
      first(provider, NC);
      break;
    case 5:
      diagram(provider, NC, spisok);
      break;
    case 6:
      dop(provider, NC);
      break;
    case 7:
      exit(0);
    }
  }// конец while(1)... 

  return 0;
}// конец main()

int menu(int n) 
{
  int y1 = 0, y2 = n - 1;//устанавливаем выделение для строчки 
  char c = 1;
  while (c != ESC) //при нажатии кнопки ESC, меню закроется 
  {
    switch (c) 
	{
    case DOWN://программирование кнопки DOWN,которая позволяет перемещаться вниз по меню

      y2 = y1;
      y1++;
      break;
    case UP://программирование кнопки UP,которая позволяет перещаться вверх по меню
      y2 = y1;
      y1--;
      break;
    case ENTER://программирование кнопки ENTER,которая позволяет выбрать вопрос 
      return y1 + 1;
    case HOME://программирование кнопки HOME,которая позволяет перейти в начало 
      y2 = y1;
      y1 = 0;
      break;
    case END://программирование кнопки END,которая позволяет перейти в конец
      y2 = y1;
      y1 = n - 1;
      break;

	 }
	//условие,когда срабатывает кнопка DOWN
    if (y1 > n - 1) 
	{
      y2 = n - 1;
      y1 = 0;
    }
	//условие,когда срабатывает кнопка UP 
    if (y1 < 0) 
	{
      y2 = 0;
      y1 = n - 1;
    }
    Console::ForegroundColor = ConsoleColor::Gray;
    Console::BackgroundColor = ConsoleColor::Red;
    Console::CursorLeft = 11;
    Console::CursorTop = y1 + 5;
    printf("%s", dan[y1]);
    Console::ForegroundColor = ConsoleColor::White;
    Console::BackgroundColor = ConsoleColor::Black;
    Console::CursorLeft = 11;
    Console::CursorTop = y2 + 5;
    printf("%s", dan[y2]);
    c = getch();

  }
  exit(0);
}
//Функция поиска максимума (максимальная цена предмета одежды)
void maximpastsum(struct z * provider, int NC) 
{
  int i = 0;
  struct z best;
  strcpy(best.vid, provider[0].vid);//в переменную структуры записываем нулевой элемент
  best.pastsumma = provider[0].pastsumma;
  for (i = 1; i < NC; i++)
    if (provider[i].pastsumma > best.pastsumma) //условие поиска максимальной розничной цены
	{
      strcpy(best.vid, provider[i].vid);
      best.pastsumma = provider[i].pastsumma;
    }
  Console::ForegroundColor = ConsoleColor::White;
  Console::BackgroundColor = ConsoleColor::DarkRed;
  Console::CursorLeft = 10;//задаем область, где будет выведено максимальная розничная цена
  Console::CursorTop = 15;
  printf("Самый дорогой предмет одежды это %s стоит %ld рублей ", best.vid, best.pastsumma);// Вывод максимальной розничной цены и сопуствующий ей предмет одежды
  getch();
}

void text_data(char * s, char * sd) 
{
  char s0[3], month[12][9] = 
  {
	  "января","февраля","марта","апреля","мая","июня","июля","августа","сентября","октября","ноября","декабря"
  };
  strcpy(s, sd + 8);
  strcat(s, " ");
  strncpy(s0, sd + 5, 2);
  s0[2] = 0;
  strcat(s, month[atoi(s0) - 1]);
  strcat(s, " ");
  strncat(s, sd, 4);
  return;
}
//Функция поиска самой ранней даты поставки(Когда был поставлен самый первый предмет одежды?)
void first(struct z * provider, int NC) 
{
  int i;
  char s[17];
  struct z * best = provider;
  for (i = 1; i < NC; i++)
    if (strcmp(provider[i].data, best -> data) < 0)//условие поиска самой ранней даты
      best = & provider[i];
  text_data(s, best -> data);
  Console::ForegroundColor = ConsoleColor::White;
  Console::BackgroundColor = ConsoleColor::DarkRed;
  Console::CursorLeft = 10;//задаем область, где будет выведен самый первый предмет одежды
  Console::CursorTop = 15;
  printf("Самый первый поставленный предмет одежды это %s ,его цена %ld рублей", best -> vid, best -> pastsumma);//вывод самого первого предмета одежды и его цены
  Console::CursorLeft = 10;
  Console::CursorTop = 16;
  printf("Поставщик %s", best -> name); //вывод поставщика
  Console::CursorLeft = 10;
  Console::CursorTop = 17;
  printf("Поставлен %s ", s);//вывод даты
  getch();
}
void vstavka(struct z * provider, char * fio, struct sp ** spisok, int NC) //функция вставки в алфавитный список
{
  int i;
  struct sp * nov, * nt, * z = 0;
  for (nt = *spisok; nt != 0 && strcmp(nt -> fio, fio) < 0; z = nt, nt = nt -> sled);//поиск места для вставки элемента в список по алфавиту
  if (nt && strcmp(nt -> fio, fio) == 0) return;
  nov = (struct sp * ) malloc(sizeof(struct sp));//выделяем память под новый элемент
  strcpy(nov -> fio, fio);//присваеваем ФИО созданному элементу
  nov -> sled = nt;//присваиваем ссылку на следующий элемент
  nov -> Pastsumma = 0;
  for (i = 0; i < NC; i++)
    if (strcmp(provider[i].name, fio) == 0)//поиск совпадения ФИО в исходных данных
      nov -> Pastsumma += provider[i].pastsumma; //подсчет суммы розничных цен этого ФИО
  if (z)
	z -> sled = nov;//если есть предыдущий элемент,то присваивается ему указатель на элемент , с которым функция работает в данный момент , то есть на след элемент
	nov -> pred = z; //присваиваем элементу, с которым функция работет сейчас, указатель на предыдущий элемент
	if (nt) nt -> pred = nov;
	nov -> sled = nt;
  if (!z) *spisok = nov;//если нет предыдущего элемента,то присваивается указатель на элемент, с которым функция работает в данный момент, то есть начало списка
    
  return;
}
//Диаграмма (соотношение розничной цены разных поставщиков)
void diagram(struct z * provider, int NC, struct sp * spisok) //создаем диаграмму
{
  struct sp * nt;//перемеенная для работы со списком 
  int len, i, NColor;//i-для номера строки
  long Pastsumma0 = 0;//число-100% 
  char str1[20];
  char str2[20];
  System::ConsoleColor Color;//Задаѐт константы, которые определяют основной цвет и цвет фона консоли
  Console::ForegroundColor = ConsoleColor::Red;//цвет символов 
  Console::BackgroundColor = ConsoleColor::Black;//цвет заднего фона 
  Console::Clear();
  Console::CursorLeft = 5;
  printf("        Поставщики       ");
  Console::CursorLeft = 5;
  Console::CursorTop = 1;
  printf("================================= ");
  Console::CursorLeft = 5;
  Console::CursorTop = 2;
  printf("  ФИО          Сумма всех вложений ");
  for (i = 0; i < NC; i++) Pastsumma0 = Pastsumma0 + provider[i].pastsumma;
  Color = ConsoleColor::Black;
  NColor = 0;
  for (nt = spisok, i = 0; nt != 0; nt = nt -> sled, i++) 
  {
    sprintf(str1, "%-10s", nt -> fio);
    sprintf(str2, "%3.1f%%", (nt -> Pastsumma * 100. / Pastsumma0));
    Console::ForegroundColor = ConsoleColor::Red;
    Console::BackgroundColor = ConsoleColor::Black;
    Console::CursorLeft = 5;
    Console::CursorTop = i + 3;
    printf(str1);
    Console::CursorLeft = 20;
    printf("%s", str2);
    Console::BackgroundColor = ++Color;
    NColor++;
    Console::CursorLeft = 30;
    for (len = 0; len < nt -> Pastsumma * 100. / Pastsumma0; len++) printf(" ");

    if (NColor == 14) //выделяем на диаграмму 14 цветов
	{
      Color = ConsoleColor::Black;
      NColor = 0;
    }
  }
  getch();
  return;
}

//Формирование списка с условием (количество предметов одежды определенного цвета)
void zvet(struct z * provider, int NC, struct Zvet ** spisok2) 
{
  int i;
  long _max = 0;
  struct Zvet * nt;
  Console::ForegroundColor = ConsoleColor::White;
  Console::BackgroundColor = ConsoleColor::DarkRed;
  Console::Clear();

  printf("\n	   Cписок количества цветов ");
  printf("\n	===============================\n");
  printf("\n	   Цвет              Кол-во ");
  printf("\n	===============================\n");
  for (nt = *spisok2; nt != 0; nt = nt -> sled_) 
  {
    if (nt -> kolvo > _max) _max = nt -> kolvo;
    printf("\n	%-20s %ld", nt -> Сolor, nt -> kolvo);
  }

  for (nt = *spisok2; nt != 0; nt = nt -> sled_)
    if (nt -> kolvo == _max) printf("\n \n	===============================\n	 Самый популярный цвет - %s", nt -> Сolor);
  getch();
}
void vstavka2(struct z * provider, char * color, int NC, struct Zvet ** spisok2)  //Вставка в список цветов
{
  int i;
  struct Zvet * nov, * nt, * z = 0;
  for (nt = *spisok2; nt != 0 && strcmp(nt -> Сolor, color) < 0; z = nt, nt = nt -> sled_);//поиск места для вставки элемента в список по алфавиту
  if (nt && strcmp(nt -> Сolor, color) == 0) return;
  nov = (struct Zvet * ) malloc(sizeof(struct sp));//выделяем память под новый элемент
  strcpy(nov -> Сolor, color);//присваеваем название цвета созданному элементу
  nov -> sled_ = nt;//присваиваем ссылку на следующий элемент
  nov -> kolvo = 0;
  for (i = 0; i < NC; i++)
    if (strcmp(provider[i].color, color) == 0)//поиск совпадения цвета в исходных данных
      nov -> kolvo += provider[i].kolvo;//подсчет 
  if (!z) *spisok2 = nov;//если нет предыдущего элемента,то присваивается указатель на элемент, с которым функция работает в данный момент, то есть начало списка
  else z -> sled_ = nov;//иначе присваивается ему указатель на элемент , с которым функция работает в данный момент , то есть на след элемент
  return;
}

//Инвертированный и алфавитный список поставщиков,сортировка всех поставщиков по алфавиту, их сумма розничных цен(функция работы с двусторонним списком)
void Alfalist(struct z * provider, int NC, struct sp * spisok) //Формирование списка
{
  int i;
  struct sp * z;
  struct sp * nt;
  Console::ForegroundColor = ConsoleColor::White;
  Console::BackgroundColor = ConsoleColor::DarkRed;
  Console::Clear();
  Console::CursorTop = 2;
  Console::CursorLeft = 29;
  printf("  Инвертированный  и алфавитный список \n");
  Console::CursorTop = 3;
  Console::CursorLeft = 3;
  printf(" ФИО         Сумма \n");
  for (nt = spisok, z = 0; nt != 0; z = nt, nt = nt -> sled);
  for (nt = z, i = 0; nt != 0; nt = nt -> pred, i++) 
	{
    printf(" %-14s  %ld \n", nt -> fio, nt -> Pastsumma);
	}

  Console::CursorTop = 3;
  Console::CursorLeft = 69;
  printf("  ФИО            Сумма \n ");
  for (nt = spisok, i = 0; nt != 0; nt = nt -> sled, i++) 
  {
    Console::CursorTop = i + 4;
    Console::CursorLeft = 69;
    printf(" %-14s  %ld \n", nt -> fio, nt -> Pastsumma);
  }
  Console::CursorTop = NC + 4;
  getch();
}

//
void dop(struct z * provider, int NC)
{
  int i, j;
  Console::CursorLeft = 10;
  Console::CursorTop = 15;
  bool has_matches = false;
  for (i = 0; i < NC; i++) 
  {
    for (j = i + 1; j < NC; j++) 
	{
      if ((provider[i].pastsumma == provider[j].pastsumma) && (strcmp(provider[i].vid, provider[j].vid) != 0)) 
	  {
        has_matches = true;
        Console::ForegroundColor = ConsoleColor::White;
        Console::BackgroundColor = ConsoleColor::DarkRed;
        printf(" Предметы одежды: %s и %s                           ", provider[i].vid, provider[j].vid);
        Console::ForegroundColor = ConsoleColor::White;
        Console::BackgroundColor = ConsoleColor::DarkRed;
        Console::CursorLeft = 10;
        Console::CursorTop = 16;
        printf(" Имеют сумму: %ld рублей                       ", provider[i].pastsumma);
        break;
      }
    }
    if (has_matches == true) break;
  }
  if (has_matches == false) 
  {
    Console::ForegroundColor = ConsoleColor::White;
    Console::BackgroundColor = ConsoleColor::DarkRed;
    printf(" Нет совпадений ");
  }
  getch();
}

