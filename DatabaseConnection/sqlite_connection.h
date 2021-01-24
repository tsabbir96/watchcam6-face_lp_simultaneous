#ifndef SQLITE_CONNECTION_H
#define SQLITE_CONNECTION_H
#include <QSqlDatabase>


static bool connectionDB()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//    db.setDatabaseName("watch_cam_ds.db");
    db.setDatabaseName("/mnt/nvme0n1p1/watch_cam_ds.db");

    if (!db.open())
        return false;

    return true;
}

#endif // SQLITE_CONNECTION_H
