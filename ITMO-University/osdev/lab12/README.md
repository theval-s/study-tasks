# Лабораторная работа 1.2 

## Сборка:
```make all```

В src.c - основной код программы, в utils.c находятся функции для поиска и открытия библиотек, а также рекурсивный проход по всем директориям.
Плагин libvsaN32471.c - ищет в бинарной форме файлах 4 подряд байта, соответсвующих заданному ip (задаётся через опцию --ipv4-addr-bin xxx.xxx.xxx.xxx).

В директории test есть file.bin в котором записаны 4 байта - C0A80301 что соответствует ip 192.168.3.1