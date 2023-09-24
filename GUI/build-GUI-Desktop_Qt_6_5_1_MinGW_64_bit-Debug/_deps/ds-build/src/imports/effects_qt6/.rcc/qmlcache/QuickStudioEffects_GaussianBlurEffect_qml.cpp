// /QtQuick/Studio/Effects/GaussianBlurEffect.qml
#include <QtQml/qqmlprivate.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qobject.h>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qurl.h>
#include <QtCore/qvariant.h>
#include <QtQml/qjsengine.h>
#include <QtQml/qjsprimitivevalue.h>
#include <QtQml/qjsvalue.h>
#include <QtQml/qqmlcomponent.h>
#include <QtQml/qqmlcontext.h>
#include <QtQml/qqmlengine.h>
#include <QtQml/qqmllist.h>
#include <qloggingcategory.h>
#include <type_traits>
namespace QmlCacheGeneratedCode {
namespace _0x5f_QtQuick_Studio_Effects_GaussianBlurEffect_qml {
extern const unsigned char qmlData alignas(16) [];
extern const unsigned char qmlData alignas(16) [] = {

0x71,0x76,0x34,0x63,0x64,0x61,0x74,0x61,
0x3b,0x0,0x0,0x0,0x1,0x5,0x6,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x40,0x5,0x0,0x0,0x36,0x31,0x37,0x36,
0x65,0x62,0x31,0x62,0x32,0x30,0x62,0x39,
0x33,0x34,0x66,0x34,0x39,0x37,0x66,0x62,
0x63,0x65,0x36,0x38,0x64,0x65,0x36,0x64,
0x35,0x37,0x34,0x31,0x37,0x34,0x66,0x35,
0x35,0x64,0x64,0x34,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x38,0x26,0xba,0x69,
0x19,0xc0,0x33,0xdc,0x5a,0x42,0x68,0x37,
0xc4,0x9b,0xf7,0x61,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x23,0x0,0x0,0x0,
0xe,0x0,0x0,0x0,0x98,0x1,0x0,0x0,
0x1,0x0,0x0,0x0,0xf8,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0xfc,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0xfc,0x0,0x0,0x0,
0x1,0x0,0x0,0x0,0xfc,0x0,0x0,0x0,
0x2,0x0,0x0,0x0,0x0,0x1,0x0,0x0,
0x0,0x0,0x0,0x0,0x8,0x1,0x0,0x0,
0x2,0x0,0x0,0x0,0x10,0x1,0x0,0x0,
0x0,0x0,0x0,0x0,0x20,0x1,0x0,0x0,
0x0,0x0,0x0,0x0,0x20,0x1,0x0,0x0,
0x0,0x0,0x0,0x0,0x20,0x1,0x0,0x0,
0x0,0x0,0x0,0x0,0x20,0x1,0x0,0x0,
0x0,0x0,0x0,0x0,0x20,0x1,0x0,0x0,
0x0,0x0,0x0,0x0,0x20,0x1,0x0,0x0,
0x0,0x0,0x0,0x0,0x20,0x1,0x0,0x0,
0xff,0xff,0xff,0xff,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0xb8,0x3,0x0,0x0,
0x20,0x1,0x0,0x0,0x88,0x1,0x0,0x0,
0xb3,0x0,0x0,0x0,0xc4,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x40,0xd5,0x3f,
0x0,0x0,0x0,0x0,0x0,0x40,0xf5,0x7f,
0x50,0x0,0x0,0x0,0x14,0x0,0x0,0x0,
0xa,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x38,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x38,0x0,0x0,0x0,0x0,0x0,0x2,0x0,
0xff,0xff,0xff,0xff,0xb,0x0,0x0,0x0,
0x28,0x0,0x50,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x7,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x28,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x1,0x0,0x0,0x0,
0x28,0x0,0x0,0x0,0x1,0x0,0x0,0x0,
0xca,0x2e,0x0,0x18,0x7,0x12,0xd,0x18,
0xa,0xac,0x1,0x7,0x1,0xa,0x18,0x6,
0xd4,0x16,0x6,0x2,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x10,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0xd0,0x1,0x0,0x0,0xd8,0x1,0x0,0x0,
0xf0,0x1,0x0,0x0,0x30,0x2,0x0,0x0,
0x48,0x2,0x0,0x0,0x68,0x2,0x0,0x0,
0x90,0x2,0x0,0x0,0xa8,0x2,0x0,0x0,
0xc0,0x2,0x0,0x0,0xd8,0x2,0x0,0x0,
0xf8,0x2,0x0,0x0,0x38,0x3,0x0,0x0,
0x50,0x3,0x0,0x0,0x60,0x3,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x7,0x0,0x0,0x0,0x51,0x0,0x74,0x0,
0x51,0x0,0x75,0x0,0x69,0x0,0x63,0x0,
0x6b,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x1a,0x0,0x0,0x0,0x51,0x0,0x74,0x0,
0x35,0x0,0x43,0x0,0x6f,0x0,0x6d,0x0,
0x70,0x0,0x61,0x0,0x74,0x0,0x2e,0x0,
0x47,0x0,0x72,0x0,0x61,0x0,0x70,0x0,
0x68,0x0,0x69,0x0,0x63,0x0,0x61,0x0,
0x6c,0x0,0x45,0x0,0x66,0x0,0x66,0x0,
0x65,0x0,0x63,0x0,0x74,0x0,0x73,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x8,0x0,0x0,0x0,0x46,0x0,0x61,0x0,
0x73,0x0,0x74,0x0,0x42,0x0,0x6c,0x0,
0x75,0x0,0x72,0x0,0x0,0x0,0x0,0x0,
0xc,0x0,0x0,0x0,0x67,0x0,0x61,0x0,
0x75,0x0,0x73,0x0,0x73,0x0,0x69,0x0,
0x61,0x0,0x6e,0x0,0x42,0x0,0x6c,0x0,
0x75,0x0,0x72,0x0,0x0,0x0,0x0,0x0,
0x11,0x0,0x0,0x0,0x74,0x0,0x72,0x0,
0x61,0x0,0x6e,0x0,0x73,0x0,0x70,0x0,
0x61,0x0,0x72,0x0,0x65,0x0,0x6e,0x0,
0x74,0x0,0x42,0x0,0x6f,0x0,0x72,0x0,
0x64,0x0,0x65,0x0,0x72,0x0,0x0,0x0,
0x6,0x0,0x0,0x0,0x72,0x0,0x61,0x0,
0x64,0x0,0x69,0x0,0x75,0x0,0x73,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x7,0x0,0x0,0x0,0x73,0x0,0x61,0x0,
0x6d,0x0,0x70,0x0,0x6c,0x0,0x65,0x0,
0x73,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x9,0x0,0x0,0x0,0x43,0x0,0x6f,0x0,
0x6d,0x0,0x70,0x0,0x6f,0x0,0x6e,0x0,
0x65,0x0,0x6e,0x0,0x74,0x0,0x0,0x0,
0xb,0x0,0x0,0x0,0x6f,0x0,0x6e,0x0,
0x43,0x0,0x6f,0x0,0x6d,0x0,0x70,0x0,
0x6c,0x0,0x65,0x0,0x74,0x0,0x65,0x0,
0x64,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x1a,0x0,0x0,0x0,0x65,0x0,0x78,0x0,
0x70,0x0,0x72,0x0,0x65,0x0,0x73,0x0,
0x73,0x0,0x69,0x0,0x6f,0x0,0x6e,0x0,
0x20,0x0,0x66,0x0,0x6f,0x0,0x72,0x0,
0x20,0x0,0x6f,0x0,0x6e,0x0,0x43,0x0,
0x6f,0x0,0x6d,0x0,0x70,0x0,0x6c,0x0,
0x65,0x0,0x74,0x0,0x65,0x0,0x64,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x7,0x0,0x0,0x0,0x63,0x0,0x6f,0x0,
0x6e,0x0,0x73,0x0,0x6f,0x0,0x6c,0x0,
0x65,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x3,0x0,0x0,0x0,0x6c,0x0,0x6f,0x0,
0x67,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x27,0x0,0x0,0x0,0x47,0x0,0x61,0x0,
0x75,0x0,0x73,0x0,0x73,0x0,0x69,0x0,
0x61,0x0,0x6e,0x0,0x42,0x0,0x6c,0x0,
0x75,0x0,0x72,0x0,0x20,0x0,0x69,0x0,
0x73,0x0,0x20,0x0,0x6e,0x0,0x6f,0x0,
0x74,0x0,0x20,0x0,0x73,0x0,0x75,0x0,
0x70,0x0,0x70,0x0,0x6f,0x0,0x72,0x0,
0x74,0x0,0x65,0x0,0x64,0x0,0x20,0x0,
0x77,0x0,0x69,0x0,0x74,0x0,0x68,0x0,
0x20,0x0,0x51,0x0,0x74,0x0,0x20,0x0,
0x36,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x2,0x0,0x0,0x0,0x10,0x0,0x0,0x0,
0x2,0x0,0x0,0x0,0x38,0x0,0x0,0x0,
0x1,0x0,0x0,0x0,0x1,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x1e,0x0,0x10,0x0,
0x0,0x2,0x0,0x0,0x1,0x0,0x0,0x0,
0x2,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x1f,0x0,0x10,0x0,0xff,0xff,0x0,0x0,
0x40,0x0,0x0,0x0,0x18,0x1,0x0,0x0,
0x3,0x0,0x0,0x0,0x4,0x0,0x0,0x0,
0x0,0x0,0xff,0xff,0xff,0xff,0xff,0xff,
0x0,0x0,0x1,0x0,0x54,0x0,0x0,0x0,
0x54,0x0,0x0,0x0,0x60,0x0,0x0,0x0,
0x1,0x0,0x0,0x0,0x74,0x0,0x0,0x0,
0x74,0x0,0x0,0x0,0x0,0x0,0x4,0x0,
0x74,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0xd4,0x0,0x0,0x0,0x21,0x0,0x10,0x0,
0x22,0x0,0x50,0x0,0xd4,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0xd4,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x7,0x0,0x0,0x0,
0x1,0x0,0x0,0x20,0x27,0x0,0x50,0x0,
0x6,0x0,0x0,0x0,0x4,0x0,0x0,0x0,
0x6,0x0,0x0,0x0,0x26,0x0,0x50,0x0,
0x26,0x0,0xc0,0x1,0x7,0x0,0x0,0x0,
0x0,0x0,0x2,0x0,0x1,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x27,0x0,0x20,0x1,
0x27,0x0,0xb0,0x1,0x6,0x0,0x0,0x0,
0x0,0x0,0x2,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x24,0x0,0x50,0x0,
0x24,0x0,0xd0,0x0,0x5,0x0,0x0,0x0,
0x0,0x0,0x1,0x0,0x1,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x23,0x0,0x50,0x0,
0x23,0x0,0x80,0x1,0x8,0x0,0x0,0x0,
0x0,0x0,0x9,0x0,0x1,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x28,0x0,0x50,0x0,
0x28,0x0,0xf0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0xff,0xff,0xff,0xff,0xff,0xff,
0x0,0x0,0x0,0x0,0x54,0x0,0x0,0x0,
0x54,0x0,0x0,0x0,0x54,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x54,0x0,0x0,0x0,
0x54,0x0,0x0,0x0,0x0,0x0,0x1,0x0,
0x54,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x6c,0x0,0x0,0x0,0x28,0x0,0x50,0x0,
0x0,0x0,0x0,0x0,0x6c,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x6c,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x9,0x0,0x0,0x0,
0x0,0x0,0x7,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x28,0x0,0xf0,0x0,
0x28,0x0,0xc0,0x1,0x0,0x0,0x0,0x0
};
QT_WARNING_PUSH
QT_WARNING_DISABLE_MSVC(4573)

template <typename Binding>
void wrapCall(const QQmlPrivate::AOTCompiledContext *aotContext, void *dataPtr, void **argumentsPtr, Binding &&binding)
{
    using return_type = std::invoke_result_t<Binding, const QQmlPrivate::AOTCompiledContext *, void **>;
    if constexpr (std::is_same_v<return_type, void>) {
       Q_UNUSED(dataPtr)
       binding(aotContext, argumentsPtr);
    } else {
        if (dataPtr) {
           new (dataPtr) return_type(binding(aotContext, argumentsPtr));
        } else {
           binding(aotContext, argumentsPtr);
        }
    }
}
extern const QQmlPrivate::TypedFunction aotBuiltFunctions[];
extern const QQmlPrivate::TypedFunction aotBuiltFunctions[] = {
{ 0, QMetaType::fromType<void>(), {  }, 
    [](const QQmlPrivate::AOTCompiledContext *aotContext, void *dataPtr, void **argumentsPtr) {
        wrapCall(aotContext, dataPtr, argumentsPtr, [](const QQmlPrivate::AOTCompiledContext *aotContext, void **argumentsPtr) {
Q_UNUSED(aotContext)
Q_UNUSED(argumentsPtr)
// expression for onCompleted at line 40, column 5
QString r2_1;
QString r10_1;
// generate_CreateCallContext
{
// generate_LoadRuntimeString
r2_1 = QStringLiteral("GaussianBlur is not supported with Qt 6");
// generate_StoreReg
r10_1 = r2_1;
// generate_CallPropertyLookup
{
    bool firstArgIsCategory = false;
    const QLoggingCategory *category = aotContext->resolveLoggingCategory(nullptr, &firstArgIsCategory);
    if (category && category->isEnabled(QtDebugMsg)) {
        const QString message = r10_1;
        aotContext->writeToConsole(QtDebugMsg, message, category);
    }
}
// generate_PopContext
;}
// generate_Ret
return;
});}
 },{ 0, QMetaType::fromType<void>(), {}, nullptr }};
QT_WARNING_POP
}
}
