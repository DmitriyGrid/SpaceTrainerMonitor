# ЛР № 6 — Тестирование модулей SpaceTrainerMonitor

Продолжение ЛР № 4 (Qt/TCP/PostgreSQL) и ЛР № 5 (алгоритмы C++17).

## Проверка алгоритмов (без Qt)

Из PowerShell, где доступны CMake, MinGW и Ninja:

```powershell
$env:Path = "C:\Qt\Tools\CMake_64\bin;C:\Qt\Tools\mingw1310_64\bin;C:\Qt\Tools\Ninja;$env:Path"
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
.\build\algorithms_extended_tests.exe
```

## Qt/TCP тесты (отдельная сборка)

Откройте CMakeLists.txt в Qt Creator с комплектом Qt 6.12 MinGW 64-bit. В конфигурации CMake добавьте `BUILD_QT_TESTS=ON`, затем соберите и запустите CTest. Либо в Qt Creator Terminal с установленным Qt пути используйте CMake `-DBUILD_QT_TESTS=ON` с `CMAKE_PREFIX_PATH=C:/Qt/6.12.0/mingw_64` и отдельным каталогом `build-qt`. Тест использует локальный порт в диапазоне 47111–47129. Освободите эти порты, если они заняты.

Qt-тест проверяет: фрагментированный TCP-пакет, правильный JSON, невалидный JSON, отсутствие поля и недопустимую длину. Тесты относятся к снимку исходного кода из ЛР №4, который включён в `reference/qt`.

## PostgreSQL

Проверка базы — ручная и **только на чтение**. Откройте SQL/04_verify_lab06.sql в pgAdmin, выполните запросы и сохраните результат. Настоящие результаты должны быть зафиксированы отдельно после запуска. Пароли и файлы конфигурации с секретами в репозиторий не добавляются.

## Что включено

- `reference/algorithms` — копия исходников ЛР №5 для воспроизводимости тестов
- `reference/qt` — копия TCP-сервера и имитатора ЛР №4
- `tests/algorithms_extended_tests.cpp` — расширенные автоматические проверки
- `tests/telemetry_qt_tests.cpp` — тесты сетевого взаимодействия (Qt Test)
- `SQL/04_verify_lab06.sql` — проверочные запросы к БД
- `Report/` — отчёт Word

**Важно:** локальный результат сборки алгоритмов не доказывает выполнение Qt-тестов и проверки PostgreSQL на компьютере студента. Эти проверки выполняются отдельно.
