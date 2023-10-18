#include <iostream>
#include <assert.h>
#include <string>
#include <leveldb/db.h>
using std::string;

int main()
{
    leveldb::DB *db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options,"/tmp/testdb",&db);
    assert(status.ok());
    string k1 = "name";
    string v1 = "jim";
    status = db->Put(leveldb::WriteOptions(),k1,v1);
    assert(status.ok());

    status = db->Get(leveldb::ReadOptions(),k1,&v1);
    assert(status.ok());
    std::cout << "k1: " << k1 << ";v1: " << v1 << std::endl;
    delete db;
    return 0;
}

