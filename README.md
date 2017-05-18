# Physics

### Файлы PhysicsF.h, PhysicsF.cpp
+ рассчетные формулы
  + 1 и 2 производные
  + скорость
  + ускорения
    + нормальное 
    + тангенциальное  
    + полное 
+ парсер мат. выражений(getTokensFromExpr, getPostfixFromExpr)
+ калькулятор(calculate)

### Файлы Physics2Dlg.h, Physics2Dlg.cpp
+ диалоговые функции
  + начальная инициализация шрифтов(CPhysics2Dlg::CPhysics2Dlg)
  + нажатие кнопки "построить"(CPhysics2Dlg::OnBnClickedCalc) (вызывает отрисовку графика выбранного в выпадающем меню)
  + переключение radio'ом режимов(CPhysics2Dlg::OnBnClickedRadio2d, CPhysics2Dlg::OnBnClickedRadioRnd)
    + 2D
    + окружность(в разработке...)
  + отрисовка графика(CPhysics2Dlg::writeGraphic)
  + отображение необходимых символов у графика(CPhysics2Dlg::SetSymb)
 
### Прочие файлы
стандартные, сгенерированы средой для рисования окон
