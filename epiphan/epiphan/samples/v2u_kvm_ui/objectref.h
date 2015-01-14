/****************************************************************************
 *
 * $Id: objectref.h 16105 2012-03-24 16:28:55Z monich $
 *
 * Copyright (C) 2012 Epiphan Systems Inc. All rights reserved.
 *
 * Support for ref-counted objects
 *
 ****************************************************************************/

#ifndef EPIPHAN_OBJECTREF_H
#define EPIPHAN_OBJECTREF_H

#include <QSharedData>

// Fields required for ref-counted object
#define OBJECT_REF_FIELDS(T) private: \
    friend class QExplicitlySharedDataPointer<T>; \
    mutable QAtomicInt ref;

// Reference to the class containing the above fields
template <class T> class ObjectRef : public QExplicitlySharedDataPointer<T> {
    typedef QExplicitlySharedDataPointer<T> BaseClass;
public:
    inline ObjectRef() : BaseClass() {}
    inline ObjectRef(T* o) : BaseClass(o) {}
    inline ObjectRef(const ObjectRef& ref) : BaseClass(ref) {}
    inline operator T*() const { return BaseClass::data(); }
    inline bool isNull() const { return BaseClass::data() == NULL; }
};

#endif // EPIPHAN_OBJECTREF_H
