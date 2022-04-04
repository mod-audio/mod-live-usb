// SPDX-FileCopyrightText: 2021-2022 Filipe Coelho <falktx@falktx.com>
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "Utils.hpp"

#include <QtCore/QDir>
#include <QtCore/QFile>

#include <dlfcn.h>

static QString getExecutableFile();
static QString getExecutableFile()
{
    struct DLAddrReader
    {
        static QString getFilename()
        {
            Dl_info exeInfo;
            void* localSymbol = (void*)getExecutableFile;
            dladdr(localSymbol, &exeInfo);
            return QString(exeInfo.dli_fname);
        }
    };

    static QString filename(DLAddrReader::getFilename());
    return filename;
}

QString findDocumentation()
{
    QFileInfo exe(getExecutableFile());
    QDir dir(exe.dir());

    printf("dir is %s\n", dir.absolutePath().toUtf8().constData());

    if (dir.exists("documentation"))
        return dir.absoluteFilePath("documentation/index.html");

    dir.cdUp();

    if (dir.exists("documentation"))
        return dir.absoluteFilePath("documentation/index.html");

    return QString();
}

QString findStartScript()
{
    QFileInfo exe(getExecutableFile());
    QDir dir(exe.dir());

    printf("dir is %s\n", dir.absolutePath().toUtf8().constData());

    if (dir.exists("start.sh"))
        return dir.filePath("start.sh");

    dir.cdUp();
    dir.cd("mod-os");

    if (dir.exists("start.sh"))
        return dir.filePath("start.sh");

    return QString();
}
