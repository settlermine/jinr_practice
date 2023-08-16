# Описание проекта
В данном проекте реализованы два файла, в рамках которых происходит обработка данных с бустера. Основой для них послужили файлы Mathcad (можно найти по ссылке https://disk.yandex.ru/d/cyRfegWX18h3Gg). Данный проект не является законченным фреймворком или библиотекой, однако функции, реализованные внутри файлов работают значительно быстрее, чем в оригинальных файлах Mathcad и могут служить основой для других проектов. Авторы проекта не обладают знаниями в области теоретической физики и физики ускорителей, поэтому некоторые комментарии и названия переменных могут быть неточными, однако результаты совпадают с оригинальными файлами.

# Работа с проектом
## Начало работы
В проекте использовался язык Python версии 3.11.4. Для корректной работы функций в проекте, потребуются такие библиотеки языка Python, как numpy, pandas, scipy и numba. Для работы тестового кода и построения графиков нужны библиотеки matplotlib, seaborn, plotly, jupyter и ipykernel. В файлах проекта есть файл requirements.txt, с помощью которого можно установить все необзодимые библиотеки нужных версий командой:
~~~shell
pip install -r requirements.txt
~~~
ПРИМЕЧАНИЕ: Перед установкой библиотек таким способом, желательно создать новое виртуальное окружение. Подробнее об этом можно почитать на официальном сайте языка Python - https://docs.python.org/3/library/venv.html. Вполне возможно, что данный проект будет работать и с другими версиями библиотек, однако это ничем не гарантируется.

## Загрузка данных
В данном проекте реализована работа с тестовыми данными, однако их размер не позволяет разместить их в репозитории Github. Их можно получить по ссылке https://disk.yandex.ru/d/IWQ2tV-awTVOFw. Для корректной работы проекта без изменения пути до файлов внутри кода, в корневой директории проекта нужно создать папку data, в которую далее нужно поместить папки booster_injection и booster_acceleration вместе с их содержимым. Чтобы использовать в проекте какие-либо другие данные, требуется поменять значения переменных, определяющих путь до файла с данными, внутри кода программы.

## Запуск кода
Чтобы запустить код, после загрузки проекта и тестовых данных нужно просто запустить все ячейки с кодом. В данном, случае, не все процессы автоматизированы, и для других данных могут потребоваться правки внутри кода (например, для лучшего расположения графиков). Однако, в сами функции и классы изменения вносить не потребуется (авторы проекта на это очень сильно рассчитывают).

## Использование функций и классов отдельно от проекта
Все функции в проекте можно использовать отдельно друг от друга, но тут потребуются некоторые пояснения. В данном проекте для ускорения работы циклов языка Python активно применялась библиотека Numba. Данная библиотека умеет "переводить" код языка Python на язык C и заранее компилировать функции для их дальнейшего использования. Если при оптимизации функции использовалась библиотека Numba, то над ней стоит декоратор `@njit`. Выглядит это, например, следующим образом:
~~~python
@njit
def __main_loop_get_beam_mass_center_positions(summary_resonace: np.ndarray, difference_resonance: np.ndarray, period_length: float) -> np.ndarray:
~~~
Если над функцией стоит такой декоратор, то изменения внутри нее следуют производить крайне осторожно, так как даже самые на первый взгляд безобидные из них могут привести к некорректной работе. Внутри данных функций можно использовать только конструкции языка Python и большинство функции библиотеки Numpy (однако, например, преобразование Фурье в ней не реализовано). Для более подробного ознакомления можно прочитать официальную документацию - https://numba.readthedocs.io/en/stable/user/5minguide.html.\
Также в файле booster_injection можно часто встретить подобные пары функций:
~~~python
@njit
def __main_loop_get_beam_mass_center_positions(summary_resonace: np.ndarray, difference_resonance: np.ndarray, period_length: float) -> np.ndarray:

def get_beam_center_positions(summary_resonace: np.ndarray, difference_resonance: np.ndarray, period_length: float) -> np.ndarray:
~~~
В данном случае, функция get_beam_center_position лишь преобразует данные в формат np.ndarray, а все вычисления выполняет функция выше. Это снова связано с особенностями работы библиотеки numba. В данном случае, внутрь функций, оптимизированных с помощью нее, требуется подовать массивы только формата np.ndarray, так как у них явно определен тип данных, что необходимо для перевода функций на язык C. Таким образом, функция get_beam_center_position предостерегает пользователя от возможного возникновения ошибки, при подачи внутрь функции, например, списка или объекта pd.Series. Лучше всегда исплользовать именно её, а не __main_loop_get_beam_mass_center_positions.\
В файле booster_acceleration также встречаются функции с декоратором njit, однако они определены, как методы класса. Они могут выглядеть следующим образом:
~~~python
@staticmethod
@njit
def correct_phases(signal: np.ndarray, signal_minimums: np.ndarray, phases: np.ndarray, harmonic_hf: int, phase_offset: int = 0) -> np.ndarray:
~~~
Как видно, здесь использовано уже два декоратора - staticmethod и njit. Декоратор staticmethod необходим для использования функций, оптимизированных с помощью библиотеки numbda внутри класса, так как без его использования, в качестве первого аргумента метода нужно обязательно указывать self - ссылку на объект класса, из-за чего будет возникать ошибка, так как numba не умеет работать с такими объектами. Таким образом, staticmethod является полностью независимым от других полей или методов класса, и его можно спокойно выносить за класс как отдельную функцию. Например, для предыдущей функции это будет выглядеть так:
~~~python
@njit
def correct_phases(signal: np.ndarray, signal_minimums: np.ndarray, phases: np.ndarray, harmonic_hf: int, phase_offset: int = 0) -> np.ndarray:
~~~
Однако, следует понимать, что внутри класса всегда используются только np.ndarray массивы. Следовательно, попадание массивов другого типа в данные методы исключено. Если выносить их в отдельные функции, то лучше делать предварительное преобразование данных, как это происходит, например, в функции get_beam_center_positions, разобранной выше.

# Авторы
- **Саматов Денис Сергеевич** (*github* - denis-samatov; *mail* - denissamatov470@gmail.com) - Перевод оригинальных файлов с Mathcad на Python, тестирование кода на правильность работы;
- **Андренков Артем Игоревич**: (*github* - settlermine; *mail* - artyom@andrenkov.ru) - оптимизация и оформление Python-кода, написание документации.

