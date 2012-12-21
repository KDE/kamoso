#ifndef EXPORTINTERFACE_H
#define EXPORTINTERFACE_H

class KJob;

class ExportInterface
{
public:

    virtual ~ExportInterface() {}

    virtual KJob* exportFiles(const QString& albumname)=0;
};

#endif // EXPORTINTERFACE_H
