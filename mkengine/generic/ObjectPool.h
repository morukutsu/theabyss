#ifndef MK_OBJ_POOL
#define MK_OBJ_POOL

#include <stdlib.h>
#include <list>

using namespace std;

template<class OPDataType> class ObjectPool {
    protected:
        OPDataType* _ObjectData;
        OPDataType** _ObjectFree;
        int _ObjectCount,_Top;
    protected:
        void FreeAll()
        {
            int i = (_ObjectCount - 1);
            for(_Top=0; _Top < _ObjectCount; _Top++){
                _ObjectFree[_Top] = &_ObjectData[i--];
            }
        }
    public:
        void FreeInstance(OPDataType* instance)
        {
            if((instance) && (_Top<_ObjectCount) && (instance>=&_ObjectData[0]) && (instance<=&_ObjectData[_ObjectCount-1])){
                _ObjectFree[_Top++] = instance;
            }
        }
        OPDataType* NewInstance()
        {
            if (_Top > 0) {
                return _ObjectFree[--_Top];
            }
            return 0;
        }
        ObjectPool(int count)
        {
            _ObjectData = new OPDataType[count];
            _ObjectFree = new OPDataType*[count];
            _ObjectCount = count;
            FreeAll();
        }
        virtual ~ObjectPool()
        {
            delete[] _ObjectData;
            delete[] _ObjectFree;
        }
};

#endif