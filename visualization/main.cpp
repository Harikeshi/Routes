#include <QApplication>

#include <Project/MainWindow.hpp>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Widgets::SceneWidget window;
    MainWindow window;
    window.resize(800, 800);

    window.show();
    return app.exec();
}

//
// #include <QtWidgets/QApplication>
// #include <QtWidgets/QWidget>
// #include <QtGui/QPainter>
// #include <QtGui/QTransform>
// #include <vector>
// #include <cmath>
// struct Vec2 {
//     double x, y;
// };
//
// static inline double clampAngle(double a) {
//     const double TWO_PI = 2.0 * M_PI;
//     a = std::fmod(a, TWO_PI);
//     if (a < 0) a += TWO_PI;
//     return a;
// }
//
// // Генерация вершин внешней ломаной-оболочки дуги окружности
// std::vector<Vec2> buildTangentEnvelopeArc(const Vec2 &C, double R,
//                                           double a0, double a1, double eps) {
//     a0 = clampAngle(a0);
//     a1 = clampAngle(a1);
//     double span = a1 - a0;
//     if (span < 0) span += 2.0 * M_PI;
//
//     const double eps_min = 1e-9;
//     if (eps < eps_min) eps = eps_min;
//
//     double cosHalf = (R / (R + eps));
//     cosHalf = std::min(1.0, std::max(0.0, cosHalf));
//     double dHalf = std::acos(cosHalf); // Δ/2
//     double dTheta = 2.0 * dHalf;
//
//     int segments = std::max(2, (int) std::ceil(span / dTheta));
//     dTheta = span / segments;
//
//     double half = 0.5 * dTheta;
//     double d = R / std::cos(half);
//
//     std::vector<Vec2> poly;
//     poly.reserve(segments);
//
//     for (int i = 0; i < segments; ++i) {
//         double theta_i = a0 + i * dTheta;
//         double theta_m = theta_i + half;
//         Vec2 V = {
//             C.x + d * std::cos(theta_m),
//             C.y + d * std::sin(theta_m)
//         };
//         poly.push_back(V);
//     }
//     return poly;
// }
//
// class TangentEnvelopeWidget : public QWidget {
//     Q_OBJECT
//
// public:
//     TangentEnvelopeWidget(QWidget *parent = nullptr) : QWidget(parent) {
//         setMinimumSize(600, 400);
//     }
//
//     void setParams(const Vec2 &C_, double R_, double a0_, double a1_, double eps_) {
//         C = C_;
//         R = R_;
//         a0 = a0_;
//         a1 = a1_;
//         eps = eps_;
//         update();
//     }
//
// protected:
//     void paintEvent(QPaintEvent *) override {
//         QPainter p(this);
//         p.setRenderHint(QPainter::Antialiasing, true);
//
//         // Вычислим трансформ: центр виджета -> C, масштаб units->pixels, инверсия Y
//         const double unitsPerPixel = 0.05; // чем меньше, тем крупнее масштаб
//         const double scale = 1.0 / unitsPerPixel;
//         QTransform T;
//         T.translate(width() * 0.5, height() * 0.5);
//         T.scale(scale, -scale); // инверсия Y, чтобы ось вверх
//         p.setWorldTransform(T);
//
//         // Фон координатной сеткой (тонкой), чтобы видеть масштаб
//         drawGrid(p);
//
//         // Дуга окружности
//         drawArc(p);
//
//         // Внешняя ломаная-оболочка
//         drawEnvelope(p);
//
//         // Центр и подсказки
//         p.setPen(QPen(QColor(60, 60, 60), 0));
//         p.setBrush(QBrush(QColor(60, 60, 60)));
//         p.drawEllipse(QPointF(C.x, C.y), 0.1, 0.1);
//     }
//
//     void drawGrid(QPainter &p) {
//         p.save();
//         p.setPen(QPen(QColor(230, 230, 230), 0));
//         const double step = 5.0;
//         // Нарисуем ограниченную сетку вокруг центра
//         for (int i = -100; i <= 100; ++i) {
//             p.drawLine(QPointF(C.x + i * step, C.y - 100 * step),
//                        QPointF(C.x + i * step, C.y + 100 * step));
//             p.drawLine(QPointF(C.x - 100 * step, C.y + i * step),
//                        QPointF(C.x + 100 * step, C.y + i * step));
//         }
//         // Оси
//         p.setPen(QPen(QColor(180, 180, 180), 0));
//         p.drawLine(QPointF(C.x - 1000, C.y), QPointF(C.x + 1000, C.y));
//         p.drawLine(QPointF(C.x, C.y - 1000), QPointF(C.x, C.y + 1000));
//         p.restore();
//     }
//
//     void drawArc(QPainter &p) {
//         p.save();
//
//         // Окружность как QRectF в мировых координатах
//         QRectF circleRect(C.x - R, C.y - R, 2 * R, 2 * R);
//
//         // Углы для QPainter: в 1/16 градуса, направление против часовой (по математике)
//         double startDeg = a0 * 180.0 / M_PI;
//         double spanDeg = (a1 - a0);
//         if (spanDeg < 0) spanDeg += 2.0 * M_PI;
//         spanDeg *= 180.0 / M_PI;
//
//         int start16 = int(std::round(startDeg * 16.0));
//         int span16 = int(std::round(spanDeg * 16.0));
//
//         // Стиль дуги
//         p.setPen(QPen(QColor(20, 140, 220), 0)); // тонкая голубая
//         p.setBrush(Qt::NoBrush);
//
//         // QPainter::drawArc ожидает прямоугольник в целевых координатах.
//         // Здесь rect в мировых, но у нас установлен worldTransform — всё ок.
//         p.drawArc(circleRect, -start16, -span16); // минусы — из-за инверсии Y
//
//         // Подпишем точки начала/конца дуги
//         QPointF P0(C.x + R * std::cos(a0), C.y + R * std::sin(a0));
//         QPointF P1(C.x + R * std::cos(a1), C.y + R * std::sin(a1));
//         p.setPen(QPen(QColor(20, 140, 220), 0));
//         p.setBrush(QBrush(QColor(20, 140, 220)));
//         p.drawEllipse(P0, 0.12, 0.12);
//         p.drawEllipse(P1, 0.12, 0.12);
//
//         p.restore();
//     }
//
//     void drawEnvelope(QPainter &p) {
//         p.save();
//
//         auto verts = buildTangentEnvelopeArc(C, R, a0, a1, eps);
//
//         QPolygonF poly;
//         poly.reserve((int) verts.size());
//         for (const auto &v: verts) poly << QPointF(v.x, v.y);
//
//         p.setPen(QPen(QColor(220, 80, 60), 0)); // тонкая красная
//         p.setBrush(Qt::NoBrush);
//         p.drawPolyline(poly);
//
//         // Рисуем касательные для первой пары как подсказку
//         if (!verts.empty()) {
//             double span = a1 - a0;
//             if (span < 0) span += 2.0 * M_PI;
//             double dTheta = span / std::max(2, (int) verts.size());
//             double theta0 = a0;
//             double theta1 = a0 + dTheta;
//             QPointF T0(C.x + R * std::cos(theta0), C.y + R * std::sin(theta0));
//             QPointF T1(C.x + R * std::cos(theta1), C.y + R * std::sin(theta1));
//
//             // Направления касательных (перпендикуляр к радиусу)
//             QPointF tdir0(-std::sin(theta0), std::cos(theta0));
//             QPointF tdir1(-std::sin(theta1), std::cos(theta1));
//
//             p.setPen(QPen(QColor(220, 160, 60), 0, Qt::DashLine));
//             const double L = R * 3;
//             p.drawLine(T0, T0 + L * tdir0);
//             p.drawLine(T1, T1 + L * tdir1);
//         }
//
//         p.restore();
//     }
//
// private:
//     Vec2 C{0.0, 0.0};
//     double R = 10.0;
//     double a0 = 0.0;
//     double a1 = M_PI / 2.0;
//     double eps = 0.1;
// };
//
// int main(int argc, char **argv) {
//     QApplication app(argc, argv);
//
//     TangentEnvelopeWidget w;
//     // Параметры из задачи:
//     w.setParams({0.0, 0.0}, 10.0, 0.0, M_PI / 2.0, 0.1);
//     w.setWindowTitle("Arc + External Tangent Polyline");
//     w.show();
//
//     return app.exec();
// }
//
// #include "main.moc"
