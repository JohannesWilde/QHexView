#pragma once

#define QHEXVIEW_VERSION 5.0

#include <QAbstractScrollArea>
#include <QTextCharFormat>
#include <QFontMetricsF>
#include <QRectF>
#include <QList>
#include "model/qhexdelegate.h"
#include "model/qhexdocument.h"
#include "model/qhexcursor.h"

class QHexView : public QAbstractScrollArea
{
    Q_OBJECT

    public:
        explicit QHexView(QWidget *parent = nullptr);
        QHexDocument* hexDocument() const;
        QHexCursor* hexCursor() const;
        const QHexMetadata* hexMetadata() const;
        QHexOptions options() const;
        QColor getReadableColor(QColor c) const;
        QByteArray selectedBytes() const;
        QByteArray getLine(qint64 line) const;
        unsigned int addressWidth() const;
        unsigned int lineLength() const;
        bool canUndo() const;
        bool canRedo() const;
        quint64 offset() const;
        quint64 address() const;
        HexPosition position() const;
        HexPosition selectionStart() const;
        HexPosition selectionEnd() const;
        quint64 selectionStartOffset() const;
        quint64 selectionEndOffset() const;
        quint64 baseAddress() const;
        quint64 lines() const;
        qint64 find(const QByteArray &ba, HexFindDirection fd = HexFindDirection::Forward) const;
        void setOptions(const QHexOptions& options);
        void setBaseAddress(quint64 baseaddress);
        void setDelegate(QHexDelegate* rd);
        void setDocument(QHexDocument* doc);
        void setData(const QByteArray& ba);
        void setData(QHexBuffer* buffer);
        void setCursorMode(QHexCursor::Mode mode);
        void setByteColor(quint8 b, QHexColor c);
        void setByteForeground(quint8 b, QColor c);
        void setByteBackground(quint8 b, QColor c);
        void setMetadata(qint64 begin, qint64 end, const QColor &fgcolor, const QColor &bgcolor, const QString &comment);
        void setForeground(qint64 begin, qint64 end, const QColor &fgcolor);
        void setBackground(qint64 begin, qint64 end, const QColor &bgcolor);
        void setComment(qint64 begin, qint64 end, const QString& comment);
        void setMetadataSize(qint64 begin, qint64 length, const QColor &fgcolor, const QColor &bgcolor, const QString &comment);
        void setForegroundSize(qint64 begin, qint64 length, const QColor &fgcolor);
        void setBackgroundSize(qint64 begin, qint64 length, const QColor &bgcolor);
        void setCommentSize(qint64 begin, qint64 length, const QString& comment);
        void removeMetadata(qint64 line);
        void removeBackground(qint64 line);
        void removeForeground(qint64 line);
        void removeComments(qint64 line);
        void unhighlight(qint64 line);
        void clearMetadata();

    public Q_SLOTS:
        void undo();
        void redo();
        void cut(bool hex = false);
        void copy(bool hex = false) const;
        void paste(bool hex = false);
        void selectAll();
        void removeSelection();
        void switchMode();
        void setAddressWidth(unsigned int w);
        void setLineLength(unsigned int l);
        void setGroupLength(unsigned int l);
        void setScrollSteps(unsigned int l);
        void setReadOnly(bool r);
        void setAutoWidth(bool r);

    private:
        void checkOptions();
        void checkState();
        void checkAndUpdate(bool calccolumns = false);
        void calcColumns();
        void ensureVisible();
        void drawSeparators(QPainter* p) const;
        void drawHeader(QTextCursor& c) const;
        void drawDocument(QTextCursor& c) const;
        QTextCharFormat drawFormat(QTextCursor& c, quint8 b, const QString& s, HexArea area, qint64 line, qint64 column, bool applyformat) const;
        unsigned int calcAddressWidth() const;
        int visibleLines(bool absolute = false) const;
        qint64 getLastColumn(qint64 line) const;
        qint64 lastLine() const;
        qreal getNCellsWidth(int n) const;
        qreal hexColumnWidth() const;
        qreal hexColumnX() const;
        qreal asciiColumnX() const;
        qreal endColumnX() const;
        qreal cellWidth() const;
        qreal lineHeight() const;
        HexPosition positionFromPoint(QPoint pt) const;
        QPoint absolutePoint(QPoint pt) const;
        HexArea areaFromPoint(QPoint pt) const;
        void moveNext(bool select = false);
        void movePrevious(bool select = false);
        bool keyPressMove(QKeyEvent* e);
        bool keyPressTextInput(QKeyEvent* e);
        bool keyPressAction(QKeyEvent* e);

    protected:
        bool event(QEvent* e) override;
        void showEvent(QShowEvent* e) override;
        void paintEvent(QPaintEvent*) override;
        void resizeEvent(QResizeEvent* e) override;
        void focusInEvent(QFocusEvent* e) override;
        void focusOutEvent(QFocusEvent* e) override;
        void mousePressEvent(QMouseEvent* e) override;
        void mouseMoveEvent(QMouseEvent* e) override;
        void wheelEvent(QWheelEvent* e) override;
        void keyPressEvent(QKeyEvent *e) override;

    private:
        static bool isColorLight(QColor c);

    Q_SIGNALS:
        void dataChanged(quint64 offset, const QByteArray& data, QHexDocument::ChangeReason reason);
        void positionChanged();
        void modeChanged();

    private:
        bool m_readonly{false}, m_writing{false}, m_autowidth{false};
        HexArea m_currentarea{HexArea::Ascii};
        QList<QRectF> m_hexcolumns;
        QFontMetricsF m_fontmetrics;
        QHexOptions m_options;
        QHexCursor* m_hexcursor{nullptr};
        QHexDocument* m_hexdocument{nullptr};
        QHexMetadata* m_hexmetadata{nullptr};
        QHexDelegate* m_hexdelegate{nullptr};
};

