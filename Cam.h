#ifndef CAM_H
#define CAM_H

class Cam {
public:
    Cam();

    //movement
    void applyView() const;
    void moveForward(float delta);
    void moveBackward(float delta);
    void moveLeft(float delta);
    void moveRight(float delta);

    //drehung
    void orbitLeft(float angleDelta);
    void orbitRight(float angleDelta);
    void zoomIn(float delta);
    void zoomOut(float delta);

    float getFov() const { return fov; }  //getter für fov!

private:
    float eyeX, eyeY, eyeZ;
    float centerX, centerY, centerZ;
    float upX, upY, upZ;

    float orbitRadius;
    float orbitAngle;
    float fov; //field of view, fürs zoomen
};

#endif