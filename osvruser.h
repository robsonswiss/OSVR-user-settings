#ifndef OSVRUSER_H
#define OSVRUSER_H

#include <QJsonObject>
#include "json/json.h"


enum eyeSide {OS,OD};

typedef struct {
    double   pupilDistance;
    bool dominant;
    struct {
        double spherical;
        double cylindrical;
        double axis;
    }correction;
    double addNear;
}eyeData;


class OSVRUser
{
public:
    OSVRUser();

    QString gender() const;
    void setGender(const QString &gender);

    void setEye(eyeSide eyeBall,
                bool dominant,
                double pupilDistance,
                double dSpherical,
                double dCylindrical,
                double dAxis);
    void setEye(eyeSide eyeBall,
                bool dominant,
                double pupilDistance,
                double dSpherical,
                double dCylindrical,
                double dAxis,
                double addNear);

    double pupilDistance() const;

    double pupilDistance(eyeSide eyeB) const;
    void setPupilDistance(eyeSide eyeBall,double ipd);
    bool dominant(eyeSide eyeB) const;
    void setDominant(eyeSide eyeBall);
    double spherical(eyeSide eyeB) const;
    void setSpherical(eyeSide eyeBall,double spherical);
    double cylindrical(eyeSide eyeB) const;
    void setCylindrical(eyeSide eyeBall,double cylindrical);
    double axis(eyeSide eyeB) const;
    void setAxis(eyeSide eyeBall,double axis);
    double addNear(eyeSide eyeB) const;
    void setAddNear(eyeSide eyeBall,double addNear);

    double standingEyeHeight() const;
    void setStandingEyeHeight(double standingEyeHeight);
    double seatedEyeHeight() const;
    void setSeatedEyeHeight(double seatedEyeHeight);
    double eyeToNeck() const;
    void setEyeToNeck(double eyeToNeck);

    void read(const Json::Value json);
    void readPersonal(const Json::Value json);
    void readEye(eyeData *e, const Json::Value json);
    void write(Json::Value &json) const;
    void writePersonal(Json::Value &personalSettingsJson) const;
    void writeEye(eyeData e,Json::Value &eyeJson) const;

private:
     QString mGender;
     eyeData mLeft;
     eyeData mRight;
     struct  anthro{
        double standingEyeHeight;
        double seatedEyeHeight;
        double eyeToNeck;
     }mAnthropometric;
};

#endif // OSVRUSER_H
