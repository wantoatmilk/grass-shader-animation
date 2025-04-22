#ifndef CAM_H
#define CAM_H

class Cam {
public:
    Cam();
    void applyView() const;

private:
    float eyeX, eyeY, eyeZ;
    float centerX, centerY, centerZ;
    float upX, upY, upZ;
};

#endif