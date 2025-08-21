#ifndef PICSTATUSBUTTON_H
#define PICSTATUSBUTTON_H

#include <QPushButton>

class PicStatusButton : public QPushButton
{
public:
    PicStatusButton(QWidget *parent = nullptr);
    void SetIcons(const QString &play_normal, const QString &play_hover, const QString &play_press,
                  const QString &pause_normal, const QString &pause_hover, const QString &pause_press);
    virtual ~PicStatusButton();

protected:
    bool event(QEvent *e) override;

private:
    void setPlayNormalIcon();
    void setPlayHoverIcon();
    void setPlayPressIcon();
    void setPauseNormalIcon();
    void setPauseHoverIcon();
    void setPausePressIcon();
    QString _play_normal;
    QString _play_hover;
    QString _play_press;
    QString _pause_normal;
    QString _pause_hover;
    QString _pause_press;
    int _cur_state;

public slots:
    void SlotStart();
    void SlotStop();
};

#endif // PICSTATUSBUTTON_H
