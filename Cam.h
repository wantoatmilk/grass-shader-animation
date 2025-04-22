#ifndef CAM_H
#define CAM_H

class Cam {
public:
    Cam();
    void applyView() const;
    void moveForward(float delta);
    void moveBackward(float delta);
    void moveLeft(float delta);
    void moveRight(float delta);

private:
    float eyeX, eyeY, eyeZ;
    float centerX, centerY, centerZ;
    float upX, upY, upZ;
};

#endif