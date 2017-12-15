/*
    Copyright (C) 2012 Collabora Ltd.
      @author George Kiagiadakis <george.kiagiadakis@collabora.co.uk>

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <gst/gst.h>
#include <gst/video/video.h>

#include <QTest>
#include <QPainter>
#include <QDebug>
#include <QWidget>
#include <QLabel>
#include <QGridLayout>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
# define SkipSingle 0
# define SkipAll 0
# define QSKIP_PORT(m, a) QSKIP(m)
#else
# define QSKIP_PORT(m, a) QSKIP(m, a)
#endif

#ifndef GST_QT_VIDEO_SINK_NO_OPENGL
# include "painters/openglsurfacepainter.h"
# include <QOpenGLWidget>
# include <QGLPixelBuffer>
#endif

#include "painters/genericsurfacepainter.h"
#include <QSignalSpy>

Q_DECLARE_METATYPE(Qt::AspectRatioMode)

struct PipelineDeleter
{
    static inline void cleanup(GstPipeline *ptr) {
        if (ptr) {
            gst_element_set_state(GST_ELEMENT(ptr), GST_STATE_NULL);
            g_object_unref(ptr);
        }
    }
};

typedef QScopedPointer<GstPipeline, PipelineDeleter> GstPipelinePtr;


struct ElementDeleter
{
    static inline void cleanup(GstElement *ptr) {
        if (ptr) {
            g_object_unref(ptr);
        }
    }
};

typedef QScopedPointer<GstElement, ElementDeleter> GstElementPtr;


struct SampleDeleter
{
    static inline void cleanup(GstSample *ptr) {
        if (ptr) {
            gst_sample_unref(ptr);
        }
    }
};

typedef QScopedPointer<GstSample, SampleDeleter> GstSamplePtr;

//------------------------------------

template <class T>
class VideoWidgetT : public T
{
public:
    explicit VideoWidgetT(QWidget *parent = 0)
        : T(parent), m_sink(NULL) {}

    virtual ~VideoWidgetT() {
        if (m_sink) {
            g_object_unref(m_sink);
        }
    }

    void setVideoSink(GstElement *sink) {
        m_sink = sink;
        g_object_connect(m_sink, "signal::update", &VideoWidgetT::onUpdate, this, NULL);
        this->setAttribute(Qt::WA_OpaquePaintEvent, true);
    }

protected:
    virtual void paintEvent(QPaintEvent*)
    {
        QPainter painter(this);
        QRect targetArea = this->rect();
        if (m_sink) {
            g_signal_emit_by_name(m_sink, "paint", (void*)&painter,
                (qreal) targetArea.x(), (qreal) targetArea.y(),
                (qreal) targetArea.width(), (qreal) targetArea.height(), NULL);
        } else {
            painter.fillRect(targetArea, Qt::black);
        }
    }

private:
    static void onUpdate(GstElement*, VideoWidgetT *self) { self->update(); }

    GstElement *m_sink;
};

typedef VideoWidgetT<QWidget> VideoWidget;

#ifndef GST_QT_VIDEO_SINK_NO_OPENGL
typedef VideoWidgetT<QOpenGLWidget> VideoGLWidget;
#endif

//------------------------------------

class QtVideoSinkTest : public QObject
{
    Q_OBJECT
public:
    explicit QtVideoSinkTest(QObject* parent = 0)
        : QObject(parent), haveArbFp(false), haveGlsl(false) {}

private Q_SLOTS:
    void initTestCase();

    void bufferFormatTest_data();
    void bufferFormatTest();

    void paintAreasTest_data();
    void paintAreasTest();

    void genericSurfacePainterFormatsTest_data();
    void genericSurfacePainterFormatsTest();

#ifndef GST_QT_VIDEO_SINK_NO_OPENGL
    void glSurfacePainterFormatsTest_data();
    void glSurfacePainterFormatsTest();
#endif

    void cleanupTestCase();

private:
    GstSample *generateTestSample(GstVideoFormat format, int pattern);
    GstPipeline *constructPipeline(GstCaps *caps, GstCaps *fakesinkCaps,
                                   bool forceAspectRatio, void *context);
    void imageCompare(const QImage & image1, const QImage & image2, const QSize & sourceSize);
    bool pixelsSimilar(const QRgb & pixel1, const QRgb & pixel2);

    bool haveArbFp;
    bool haveGlsl;
};

//------------------------------------

void QtVideoSinkTest::initTestCase()
{
    gst_init(NULL, NULL);

#ifndef GST_QT_VIDEO_SINK_NO_OPENGL
    // this is just to create a gl context
    QGLPixelBuffer pixelBuffer(100, 100);
    pixelBuffer.makeCurrent();

    const QByteArray extensions(reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS)));

# ifndef QT_OPENGL_ES
    haveArbFp = extensions.contains("ARB_fragment_program");
# endif

# ifndef QT_OPENGL_ES_2
    haveGlsl = QOpenGLShaderProgram::hasOpenGLShaderPrograms()
                && extensions.contains("ARB_shader_objects");
# else
    haveGlsl = true;
# endif
#endif
}

void QtVideoSinkTest::cleanupTestCase()
{
    gst_deinit();
}

//------------------------------------

void QtVideoSinkTest::bufferFormatTest_data()
{
    QTest::addColumn<GstVideoFormat>("format");
    QTest::addColumn<GstVideoColorMatrix>("colorMatrix");

    QTest::newRow("ARGB") << GST_VIDEO_FORMAT_ARGB << GST_VIDEO_COLOR_MATRIX_RGB;
    QTest::newRow("BGRA") << GST_VIDEO_FORMAT_BGRA << GST_VIDEO_COLOR_MATRIX_RGB;
    QTest::newRow("xRGB") << GST_VIDEO_FORMAT_xRGB << GST_VIDEO_COLOR_MATRIX_RGB;
    QTest::newRow("BGRx") << GST_VIDEO_FORMAT_BGRx << GST_VIDEO_COLOR_MATRIX_RGB;
    QTest::newRow("RGB16") << GST_VIDEO_FORMAT_RGB16 << GST_VIDEO_COLOR_MATRIX_RGB;
    QTest::newRow("BGR16") << GST_VIDEO_FORMAT_BGR16 << GST_VIDEO_COLOR_MATRIX_RGB;
    QTest::newRow("RGB") << GST_VIDEO_FORMAT_RGB << GST_VIDEO_COLOR_MATRIX_RGB;
    QTest::newRow("BGR") << GST_VIDEO_FORMAT_BGR << GST_VIDEO_COLOR_MATRIX_RGB;
    QTest::newRow("I420") << GST_VIDEO_FORMAT_I420 << GST_VIDEO_COLOR_MATRIX_BT601;
    QTest::newRow("YV12") << GST_VIDEO_FORMAT_YV12 << GST_VIDEO_COLOR_MATRIX_BT601;
    QTest::newRow("v308") << GST_VIDEO_FORMAT_v308 << GST_VIDEO_COLOR_MATRIX_BT601;
    QTest::newRow("AYUV") << GST_VIDEO_FORMAT_AYUV << GST_VIDEO_COLOR_MATRIX_BT601;
}

void QtVideoSinkTest::bufferFormatTest()
{
    QFETCH(GstVideoFormat, format);
    QFETCH(GstVideoColorMatrix, colorMatrix);

    GstCaps *caps = BufferFormat::newCaps(format, QSize(100, 200), Fraction(3, 4), Fraction(5, 6));
    BufferFormat bufferFormat = BufferFormat::fromCaps(caps);
    gst_caps_unref(caps);

    QCOMPARE(bufferFormat.videoFormat(), format);
    QCOMPARE(bufferFormat.colorMatrix(), colorMatrix);
    QCOMPARE(bufferFormat.frameSize(), QSize(100, 200));
    QCOMPARE(bufferFormat.pixelAspectRatio(), Fraction(5, 6));
}

//------------------------------------

void QtVideoSinkTest::paintAreasTest_data()
{
    QTest::addColumn<QRectF>("targetArea");
    QTest::addColumn<QSize>("frameSize");
    QTest::addColumn<Fraction>("pixelAspectRatio");
    QTest::addColumn<Fraction>("displayAspectRatio");
    QTest::addColumn<QRectF>("blackArea1");
    QTest::addColumn<QRectF>("videoArea");
    QTest::addColumn<QRectF>("blackArea2");
    QTest::addColumn<Qt::AspectRatioMode>("mode");

    QTest::newRow("targetArea == videoArea")
        << QRectF(0.0, 0.0, 320.0, 240.0)
        << QSize(320, 240) << Fraction(1, 1) << Fraction(1, 1)
        << QRectF(0.0, 0.0, 0.0, 0.0)
        << QRectF(0.0, 0.0, 320.0, 240.0)
        << QRectF(0.0, 0.0, 0.0, 0.0)
        << Qt::KeepAspectRatio;

    QTest::newRow("targetArea wide")
        << QRectF(0.0, 0.0, 400.0, 240.0)
        << QSize(320, 240) << Fraction(1, 1) << Fraction(1, 1)
        << QRectF(0.0, 0.0, 40.0, 240.0)
        << QRectF(40.0, 0.0, 320.0, 240.0)
        << QRectF(360.0, 0.0, 40.0, 240.0)
        << Qt::KeepAspectRatio;

    QTest::newRow("targetArea tall")
        << QRectF(0.0, 0.0, 320.0, 300.0)
        << QSize(320, 240) << Fraction(1, 1) << Fraction(1, 1)
        << QRectF(0.0, 0.0, 320.0, 30.0)
        << QRectF(0.0, 30.0, 320.0, 240.0)
        << QRectF(0.0, 270.0, 320.0, 30.0)
        << Qt::KeepAspectRatio;


    QTest::newRow("targetArea == videoArea @ (2, 3)")
        << QRectF(2.0, 3.0, 320.0, 240.0)
        << QSize(320, 240) << Fraction(1, 1) << Fraction(1, 1)
        << QRectF(0.0, 0.0, 0.0, 0.0)
        << QRectF(2.0, 3.0, 320.0, 240.0)
        << QRectF(0.0, 0.0, 0.0, 0.0)
        << Qt::KeepAspectRatio;

    QTest::newRow("targetArea wide @ (2, 3)")
        << QRectF(2.0, 3.0, 400.0, 240.0)
        << QSize(320, 240) << Fraction(1, 1) << Fraction(1, 1)
        << QRectF(2.0, 3.0, 40.0, 240.0)
        << QRectF(42.0, 3.0, 320.0, 240.0)
        << QRectF(362.0, 3.0, 40.0, 240.0)
        << Qt::KeepAspectRatio;

    QTest::newRow("targetArea tall @ (2, 3)")
        << QRectF(2.0, 3.0, 320.0, 300.0)
        << QSize(320, 240) << Fraction(1, 1) << Fraction(1, 1)
        << QRectF(2.0, 3.0, 320.0, 30.0)
        << QRectF(2.0, 33.0, 320.0, 240.0)
        << QRectF(2.0, 273.0, 320.0, 30.0)
        << Qt::KeepAspectRatio;


    QTest::newRow("targetArea.size() == videoSize w/ par 2/1")
        << QRectF(0.0, 0.0, 160.0, 240.0)
        << QSize(160, 240) << Fraction(2, 1) << Fraction(1, 1)
        << QRectF(0.0, 0.0, 160.0, 60.0)
        << QRectF(0.0, 60.0, 160.0, 120.0)
        << QRectF(0.0, 180.0, 160.0, 60.0)
        << Qt::KeepAspectRatio;

    QTest::newRow("dar 2/1")
        << QRectF(0.0, 0.0, 160.0, 240.0)
        << QSize(320, 240) << Fraction(1, 1) << Fraction(2, 1)
        << QRectF(0.0, 0.0, 0.0, 0.0)
        << QRectF(0.0, 0.0, 160.0, 240.0)
        << QRectF(0.0, 0.0, 0.0, 0.0)
        << Qt::KeepAspectRatio;
}

void QtVideoSinkTest::paintAreasTest()
{
    QFETCH(QRectF, targetArea);
    QFETCH(QSize, frameSize);
    QFETCH(Fraction, pixelAspectRatio);
    QFETCH(Fraction, displayAspectRatio);
    QFETCH(QRectF, blackArea1);
    QFETCH(QRectF, videoArea);
    QFETCH(QRectF, blackArea2);
    QFETCH(Qt::AspectRatioMode, mode);

    PaintAreas areas;
    areas.calculate(targetArea, frameSize, pixelAspectRatio, displayAspectRatio, mode);
    QCOMPARE(areas.targetArea, targetArea);
    QCOMPARE(areas.videoArea, videoArea);
    QCOMPARE(areas.blackArea1, blackArea1);
    QCOMPARE(areas.blackArea2, blackArea2);
}

//------------------------------------

void QtVideoSinkTest::genericSurfacePainterFormatsTest_data()
{
    QTest::addColumn<GstVideoFormat>("format");

    QSet<GstVideoFormat> formats = GenericSurfacePainter::supportedPixelFormats();
    GEnumClass *gstVideoFormatClass = G_ENUM_CLASS(g_type_class_ref(GST_TYPE_VIDEO_FORMAT));

    Q_FOREACH(GstVideoFormat format, formats) {
        GEnumValue *value = g_enum_get_value(gstVideoFormatClass, format);
        QTest::newRow(value->value_name) << format;
    }

    g_type_class_unref(gstVideoFormatClass);
}

void QtVideoSinkTest::genericSurfacePainterFormatsTest()
{
    QFETCH(GstVideoFormat, format);
    QVERIFY(format != GST_VIDEO_FORMAT_UNKNOWN);

    GstCaps *caps = BufferFormat::newCaps(format, QSize(100, 100), Fraction(1, 1), Fraction(1, 1));
    BufferFormat bufferFormat = BufferFormat::fromCaps(caps);
    gst_caps_unref(caps);

    PaintAreas areas;
    areas.targetArea = QRectF(QPointF(0,0), bufferFormat.frameSize());
    areas.videoArea = areas.targetArea;
    areas.sourceRect = QRectF(0, 0, 1, 1);

    GenericSurfacePainter genericSurfacePainter;
    QVERIFY(genericSurfacePainter.supportsFormat(format));
    try {
        genericSurfacePainter.init(bufferFormat);
    } catch (const QString & error) {
        QFAIL("Failed to initialize GenericSurfacePainter");
    }

    QImage targetImage(QSize(100, 100), QImage::Format_ARGB32);
    targetImage.fill(Qt::black);
    QPainter painter(&targetImage);

    GstSamplePtr sample(generateTestSample(format, 4)); //pattern = red
    QVERIFY(!sample.isNull());
    GstBuffer *buffer = gst_sample_get_buffer(sample.data());
    QVERIFY(buffer);
    GstMapInfo info;
    QVERIFY(gst_buffer_map(buffer, &info, GST_MAP_READ));
    genericSurfacePainter.paint(
        info.data,
        bufferFormat,
        &painter,
        areas);
    QCOMPARE(targetImage.pixel(50, 50), qRgb(255, 0, 0));
    gst_buffer_unmap(buffer, &info);

    sample.reset(generateTestSample(format, 5)); //pattern = green
    QVERIFY(!sample.isNull());
    buffer = gst_sample_get_buffer(sample.data());
    QVERIFY(buffer);
    QVERIFY(gst_buffer_map(buffer, &info, GST_MAP_READ));
    genericSurfacePainter.paint(
        info.data,
        bufferFormat,
        &painter,
        areas);
    QCOMPARE(targetImage.pixel(50, 50), qRgb(0, 255, 0));
    gst_buffer_unmap(buffer, &info);

    sample.reset(generateTestSample(format, 6)); //pattern = blue
    QVERIFY(!sample.isNull());
    buffer = gst_sample_get_buffer(sample.data());
    QVERIFY(buffer);
    QVERIFY(gst_buffer_map(buffer, &info, GST_MAP_READ));
    genericSurfacePainter.paint(
        info.data,
        bufferFormat,
        &painter,
        areas);
    QCOMPARE(targetImage.pixel(50, 50), qRgb(0, 0, 255));


    QBENCHMARK {
        genericSurfacePainter.paint(
            info.data,
            bufferFormat,
            &painter,
            areas);
    }
    gst_buffer_unmap(buffer, &info);
}

//------------------------------------

#ifndef GST_QT_VIDEO_SINK_NO_OPENGL

void QtVideoSinkTest::glSurfacePainterFormatsTest_data()
{
    QTest::addColumn<GstVideoFormat>("format");
    QTest::addColumn<bool>("glsl");


    QSet<GstVideoFormat> formats = OpenGLSurfacePainter::supportedPixelFormats();
    GEnumClass *gstVideoFormatClass = G_ENUM_CLASS(g_type_class_ref(GST_TYPE_VIDEO_FORMAT));

    Q_FOREACH(GstVideoFormat format, formats) {
        GEnumValue *value = g_enum_get_value(gstVideoFormatClass, format);
        QTest::newRow(QByteArray("glsl ") + value->value_name) << format << true;
        QTest::newRow(QByteArray("arbfp ") + value->value_name) << format << false;
    }

    g_type_class_unref(gstVideoFormatClass);
}

void QtVideoSinkTest::glSurfacePainterFormatsTest()
{
    QFETCH(GstVideoFormat, format);
    QFETCH(bool, glsl);
    QVERIFY(format != GST_VIDEO_FORMAT_UNKNOWN);

    if (glsl && !haveGlsl) {
        QSKIP_PORT("Skipping because the system does not support GLSL", SkipSingle);
    }

    if (!glsl && !haveArbFp) {
        QSKIP_PORT("Skipping because the system does not support ARB Fragment Programs", SkipSingle);
    }

    GstCaps *caps = BufferFormat::newCaps(format, QSize(100, 100), Fraction(1, 1), Fraction(1, 1));
    BufferFormat bufferFormat = BufferFormat::fromCaps(caps);
    gst_caps_unref(caps);

    PaintAreas areas;
    areas.targetArea = QRectF(QPointF(0,0), bufferFormat.frameSize());
    areas.videoArea = areas.targetArea;
    areas.sourceRect = QRectF(0, 0, 1, 1);

    QGLPixelBuffer pixelBuffer(100, 100);
    pixelBuffer.makeCurrent();

    QScopedPointer<AbstractSurfacePainter> glSurfacePainter;
    if (glsl) {
        glSurfacePainter.reset(new GlslSurfacePainter);
    } else {
#ifndef QT_OPENGL_ES
        glSurfacePainter.reset(new ArbFpSurfacePainter);
#endif
    }

    QVERIFY(glSurfacePainter->supportsFormat(format));

    try {
        glSurfacePainter->init(bufferFormat);
    } catch (const QString & error) {
        QFAIL("Failed to initialize OpenGLSurfacePainter");
    }

    glSurfacePainter->updateColors(0, 0, 0, 0);
    QPainter painter(&pixelBuffer);

    GstSamplePtr sample(generateTestSample(format, 4)); //pattern = red
    QVERIFY(!sample.isNull());
    GstMapInfo info;
    GstBuffer *buffer = gst_sample_get_buffer(sample.data());
    QVERIFY(buffer);

    QVERIFY(gst_buffer_map(buffer, &info, GST_MAP_READ));
    glSurfacePainter->paint(
        info.data,
        bufferFormat,
        &painter,
        areas);
    QRgb pixel1 = pixelBuffer.toImage().pixel(50, 50);
    QRgb pixel2 = qRgb(255, 0, 0);
    if (!pixelsSimilar(pixel1, pixel2)) {
        qWarning("Found difference (%d, %d, %d) vs (%d, %d, %d)",
                qRed(pixel1), qGreen(pixel1), qBlue(pixel1),
                qRed(pixel2), qGreen(pixel2), qBlue(pixel2));
        QFAIL("Failing due to differences in the compared images");
    }
    gst_buffer_unmap(buffer, &info);

    sample.reset(generateTestSample(format, 5)); //pattern = green
    QVERIFY(!sample.isNull());
    buffer = gst_sample_get_buffer(sample.data());
    QVERIFY(buffer);
    QVERIFY(gst_buffer_map(buffer, &info, GST_MAP_READ));
    glSurfacePainter->paint(
        info.data,
        bufferFormat,
        &painter,
        areas);
    pixel1 = pixelBuffer.toImage().pixel(50, 50);
    pixel2 = qRgb(0, 255, 0);
    if (!pixelsSimilar(pixel1, pixel2)) {
        qWarning("Found difference (%d, %d, %d) vs (%d, %d, %d)",
                qRed(pixel1), qGreen(pixel1), qBlue(pixel1),
                qRed(pixel2), qGreen(pixel2), qBlue(pixel2));
    }
    gst_buffer_unmap(buffer, &info);

    sample.reset(generateTestSample(format, 6)); //pattern = blue
    QVERIFY(!sample.isNull());
    buffer = gst_sample_get_buffer(sample.data());
    QVERIFY(buffer);
    QVERIFY(gst_buffer_map(buffer, &info, GST_MAP_READ));
    glSurfacePainter->paint(
        info.data,
        bufferFormat,
        &painter,
        areas);
    pixel1 = pixelBuffer.toImage().pixel(50, 50);
    pixel2 = qRgb(0, 0, 255);
    if (!pixelsSimilar(pixel1, pixel2)) {
        qWarning("Found difference (%d, %d, %d) vs (%d, %d, %d)",
                qRed(pixel1), qGreen(pixel1), qBlue(pixel1),
                qRed(pixel2), qGreen(pixel2), qBlue(pixel2));
    }


    QBENCHMARK {
        glSurfacePainter->paint(
            info.data,
            bufferFormat,
            &painter,
            areas);
    }
    gst_buffer_unmap(buffer, &info);

}

#endif

//------------------------------------

struct ColorsTuple
{
    ColorsTuple()
        : contrast(0), brightness(0), hue(0), saturation(0) {}

    void randomize() {
        contrast = rand() % 200 - 100;
        brightness = rand() % 200 -100;
        hue = rand() % 200 -100;
        saturation = rand() % 200 -100;
    }

    int contrast;
    int brightness;
    int hue;
    int saturation;
};

//------------------------------------

#define MAKE_ELEMENT(variable, name) \
    GstElement *variable = gst_element_factory_make(name, #variable); \
    if (!variable) { \
        QWARN("Failed to create " name " with name " #variable); \
        return NULL; \
    } else { \
        gst_bin_add(GST_BIN(pipeline.data()), variable); \
    }

GstSample* QtVideoSinkTest::generateTestSample(GstVideoFormat format, int pattern)
{
    GstPipelinePtr pipeline(GST_PIPELINE(gst_pipeline_new("generate-test-sample-pipeline")));
    if (!pipeline) {
        return NULL;
    }

    MAKE_ELEMENT(videotestsrc, "videotestsrc");
    MAKE_ELEMENT(videoconvert, "videoconvert");
    MAKE_ELEMENT(capsfilter, "capsfilter");
    MAKE_ELEMENT(fakesink, "fakesink");

    GstCaps *caps = BufferFormat::newCaps(format, QSize(100, 100), Fraction(1, 1), Fraction(1, 1));
    g_object_set(capsfilter, "caps", caps, NULL);
    gst_caps_unref(caps);

    g_object_set(videotestsrc, "pattern", pattern, NULL);
    g_object_set(fakesink, "enable-last-sample", TRUE, NULL);

    if (!gst_element_link_many(videotestsrc, videoconvert, capsfilter, fakesink, NULL)) {
        QWARN("Failed to link generate-test-sample-pipeline");
        return NULL;
    }

    gst_element_set_state(GST_ELEMENT(pipeline.data()), GST_STATE_PAUSED);

    GstState state = GST_STATE_NULL;
    GstStateChangeReturn stateReturn = gst_element_get_state(GST_ELEMENT(pipeline.data()),
                                                             &state, NULL, 10 * GST_SECOND);
    if (stateReturn != GST_STATE_CHANGE_SUCCESS || state != GST_STATE_PAUSED) {
        QWARN("Failed to set generate-test-sample-pipeline to PAUSED");
        return NULL;
    }

    GstSample *samplePtr = NULL;
    g_object_get(fakesink, "last-sample", &samplePtr, NULL);
    return samplePtr;
}

GstPipeline *QtVideoSinkTest::constructPipeline(GstCaps *caps,
        GstCaps *fakesinkCaps, bool forceAspectRatio, void *context)
{
    GstPipelinePtr pipeline(GST_PIPELINE(gst_pipeline_new("test-pipeline")));
    if (!pipeline) {
        return NULL;
    }

    MAKE_ELEMENT(videotestsrc, "videotestsrc");
    MAKE_ELEMENT(capsfilter, "capsfilter");
    MAKE_ELEMENT(tee, "tee");

    MAKE_ELEMENT(queue, "queue");
    MAKE_ELEMENT(qt5videosink, "qtquick2videosink");

    MAKE_ELEMENT(queue2, "queue");
    MAKE_ELEMENT(colorspace, "videoconvert");
    MAKE_ELEMENT(videoscale, "videoscale");
    MAKE_ELEMENT(capsfilter2, "capsfilter");
    MAKE_ELEMENT(fakesink, "fakesink");

    g_object_set(videotestsrc, "pattern", 19, NULL); //color bars
    g_object_set(capsfilter, "caps", caps, NULL);
    g_object_set(capsfilter2, "caps", fakesinkCaps, NULL);
    g_object_set(fakesink, "enable-last-sample", TRUE, NULL);

    if (context) {
        g_object_set(qt5videosink, "glcontext", context, NULL);
    }

    g_object_set(qt5videosink, "force-aspect-ratio", (gboolean) forceAspectRatio, NULL);
    g_object_set(videoscale, "add-borders", (gboolean) forceAspectRatio, NULL);

    if (!gst_element_link_many(videotestsrc, capsfilter, tee, NULL)) {
        QWARN("Failed to link upstream elements");
        return NULL;
    }

    if (!gst_element_link(queue, qt5videosink)) {
        QWARN("Failed to link qt5videosink branch");
        return NULL;
    }

    if (!gst_element_link_pads(tee, "src_%u", queue, "sink")) {
        QWARN("Failed to link tee to qt5videosink");
        return NULL;
    }

    if (!gst_element_link_many(queue2, colorspace, videoscale, capsfilter2, fakesink, NULL)) {
        QWARN("Failed to link fakesink branch");
        return NULL;
    }

    if (!gst_element_link_pads(tee, "src_%u", queue2, "sink")) {
        QWARN("Failed to link tee to fakesink branch");
        return NULL;
    }

    return GST_PIPELINE(g_object_ref(pipeline.data()));
}

void QtVideoSinkTest::imageCompare(const QImage & image1, const QImage & image2, const QSize & sourceSize)
{
    QVERIFY(image1.size() == image2.size());

    QVector<int> hStartStopPoints;
    QVector<int> vStartStopPoints;

    QRect barsArea;
    if (sourceSize.isValid() && sourceSize != image1.size()) {
        PaintAreas areas;
        areas.calculate(image1.rect(), sourceSize, Fraction(1,1), Fraction(1,1), Qt::KeepAspectRatio);
        barsArea = areas.videoArea.toRect();
    } else {
        barsArea = image1.rect();
    }

    // do not compare scaling artifacts - this algorithm depends on videotestsrc's pattern 19
    qreal colorChangePoint = barsArea.width() / 7.0;

    // omit 9% of the pixels before and after the color change
    int area = qRound(colorChangePoint * 0.09);
    hStartStopPoints.append(barsArea.left() + 2);
    for (int i = 1; i < 7; i++) {
        hStartStopPoints.append(barsArea.left() + int(colorChangePoint * i) - area);
        hStartStopPoints.append(barsArea.left() + int(colorChangePoint * i) + area + 1);
    }
    hStartStopPoints.append(barsArea.right() - 1);

    vStartStopPoints.append(barsArea.top() + 2);
    vStartStopPoints.append(barsArea.bottom() - 1);

    for (int hsspCounter = 0; hsspCounter < hStartStopPoints.size() - 1; hsspCounter += 2) {
        for (int i = hStartStopPoints.at(hsspCounter); i < hStartStopPoints.at(hsspCounter+1); i++) {
            for (int vsspCounter = 0; vsspCounter < vStartStopPoints.size() - 1; vsspCounter += 2) {
                for (int j = vStartStopPoints.at(vsspCounter); j < vStartStopPoints.at(vsspCounter+1); j++) {
                    QRgb pixel1 = image1.pixel(i, j);
                    QRgb pixel2 = image2.pixel(i, j);

                    //yuv formats are lossy, therefore we cannot use pixel1 == pixel2
                    if (!pixelsSimilar(pixel1, pixel2)) {
                        qWarning("Found difference at %d, %d (%d, %d, %d) vs (%d, %d, %d)", i, j,
                                qRed(pixel1), qGreen(pixel1), qBlue(pixel1),
                                qRed(pixel2), qGreen(pixel2), qBlue(pixel2));
                        QFAIL("Failing due to differences in the compared images");
                    }
                }
            }
        }
    }
}

bool QtVideoSinkTest::pixelsSimilar(const QRgb & pixel1, const QRgb & pixel2)
{
    return !(qAbs(qRed(pixel1) - qRed(pixel2)) > 5 ||
        qAbs(qGreen(pixel1) - qGreen(pixel2)) > 5 ||
        qAbs(qBlue(pixel1) - qBlue(pixel2)) > 5);
}

QTEST_MAIN(QtVideoSinkTest)
#include "autotest.moc"
