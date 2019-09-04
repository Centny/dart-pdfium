#include <string.h>
#include <stdio.h>
#include "dart_api.h"
#include "fpdfview.h"
#include "fpdf_thumbnail.h"
#include "fpdf_text.h"
#include "fpdf_doc.h"
// Forward declaration of ResolveName function.
Dart_NativeFunction DP_ResolveName(Dart_Handle name, int argc, bool *auto_setup_scope);

#define NativeArgPeer(name, index, required)                                                                       \
    void *name = NULL;                                                                                             \
    Dart_Handle name##_h = Dart_GetNativeArgument(arguments, index);                                               \
    if (Dart_IsNull(name##_h))                                                                                     \
    {                                                                                                              \
        if (required)                                                                                              \
        {                                                                                                          \
            Dart_SetReturnValue(arguments, Dart_NewUnhandledExceptionError(Dart_NewApiError("missing arugment"))); \
            return;                                                                                                \
        }                                                                                                          \
    }                                                                                                              \
    else                                                                                                           \
    {                                                                                                              \
        Dart_Handle name##_t = Dart_GetPeer(name##_h, &name);                                                      \
        if (Dart_IsError(name##_t))                                                                                \
        {                                                                                                          \
            Dart_SetReturnValue(arguments, Dart_NewUnhandledExceptionError(name##_t));                             \
            return;                                                                                                \
        }                                                                                                          \
    }

#define NativeArgInt64(name, index)                                                \
    int64_t name = 0;                                                              \
    Dart_Handle name##_h = Dart_GetNativeArgument(arguments, index);               \
    Dart_Handle name##_t = Dart_IntegerToInt64(name##_h, &name);                   \
    if (Dart_IsError(name##_t))                                                    \
    {                                                                              \
        Dart_SetReturnValue(arguments, Dart_NewUnhandledExceptionError(name##_t)); \
        return;                                                                    \
    }

#define NativeArgCString(name, index, required)                                                                    \
    const char *name = NULL;                                                                                       \
    Dart_Handle name##_h = Dart_GetNativeArgument(arguments, index);                                               \
    if (Dart_IsNull(name##_h))                                                                                     \
    {                                                                                                              \
        if (required)                                                                                              \
        {                                                                                                          \
            Dart_SetReturnValue(arguments, Dart_NewUnhandledExceptionError(Dart_NewApiError("missing arugment"))); \
            return;                                                                                                \
        }                                                                                                          \
    }                                                                                                              \
    else                                                                                                           \
    {                                                                                                              \
        Dart_Handle name##_t = Dart_StringToCString(name##_h, &name);                                              \
        if (Dart_IsError(name##_t))                                                                                \
        {                                                                                                          \
            Dart_SetReturnValue(arguments, Dart_NewUnhandledExceptionError(name##_t));                             \
            return;                                                                                                \
        }                                                                                                          \
    }

#define NativeArgUTF16String(name, index)                                          \
    uint16_t name[1024];                                                           \
    intptr_t name##_len = 1024;                                                    \
    Dart_Handle name##_h = Dart_GetNativeArgument(arguments, index);               \
    Dart_Handle name##_t = Dart_StringToUTF16(name##_h, name, &name##_len);        \
    if (Dart_IsError(name##_t))                                                    \
    {                                                                              \
        Dart_SetReturnValue(arguments, Dart_NewUnhandledExceptionError(name##_t)); \
        return;                                                                    \
    }                                                                              \
    name[name##_len] = 0;

#define NativeArgDouble(name, index)                                               \
    double name = 0;                                                               \
    Dart_Handle name##_h = Dart_GetNativeArgument(arguments, index);               \
    Dart_Handle name##_t = Dart_DoubleValue(name##_h, &name);                      \
    if (Dart_IsError(name##_t))                                                    \
    {                                                                              \
        Dart_SetReturnValue(arguments, Dart_NewUnhandledExceptionError(name##_t)); \
        return;                                                                    \
    }

// The name of the initialization function is the extension name followed
// by _Init.
DART_EXPORT Dart_Handle dart_pdfium_Init(Dart_Handle parent_library)
{
    if (Dart_IsError(parent_library))
        return parent_library;

    Dart_Handle result_code =
        Dart_SetNativeResolver(parent_library, DP_ResolveName, NULL);
    if (Dart_IsError(result_code))
        return result_code;

    return Dart_Null();
}

bool DP_CheckError(Dart_Handle handle)
{
    if (Dart_IsError(handle))
    {
        Dart_PropagateError(handle);
        return true;
    }
    return false;
}

// Dart_Handle DP_HandleError(Dart_Handle handle)
// {
//     if (Dart_IsError(handle))
//     {
//         Dart_PropagateError(handle);
//     }
//     return handle;
// }

Dart_Handle DP_NewObject()
{
    Dart_Handle library = Dart_LookupLibrary(Dart_NewStringFromCString("dart:core"));
    Dart_Handle name = Dart_NewStringFromCString("Object");
    Dart_Handle type = Dart_GetType(library, name, 0, NULL);
    return Dart_New(type, Dart_Null(), 0, NULL);
}

// Dart_Handle DP_LastError()
// {
//     Dart_Handle result = Dart_Null();
//     unsigned long err = FPDF_GetLastError();
//     switch (err)
//     {
//     case FPDF_ERR_SUCCESS:
//         result = Dart_Null();
//         break;
//     case FPDF_ERR_UNKNOWN:
//         result = Dart_NewApiError("Unknown error");
//         break;
//     case FPDF_ERR_FILE:
//         result = Dart_NewApiError("File not found or could not be opened");
//         break;
//     case FPDF_ERR_FORMAT:
//         result = Dart_NewApiError("File not in PDF format or corrupted");
//         break;
//     case FPDF_ERR_PASSWORD:
//         result = Dart_NewApiError("Password required or incorrect password");
//         break;
//     case FPDF_ERR_SECURITY:
//         result = Dart_NewApiError("Unsupported security scheme");
//         break;
//     case FPDF_ERR_PAGE:
//         result = Dart_NewApiError("Page not found or content error");
//         break;
//     default:
//         char buf[1024];
//         int l = sprintf(buf, "Unknown error %ld", err);
//         buf[l] = 0;
//         result = Dart_NewApiError(buf);
//     }
//     return result;
// }

void DP_LastError(Dart_NativeArguments arguments)
{
    Dart_SetReturnValue(arguments, Dart_NewInteger(FPDF_GetLastError()));
}

void DP_InitLibrary(Dart_NativeArguments arguments)
{
    FPDF_InitLibrary();
}

void DP_InitLibraryWithConfig(Dart_NativeArguments arguments)
{
    NativeArgInt64(version, 0);
    NativeArgCString(font, 1, 0);
    FPDF_LIBRARY_CONFIG config;
    config.version = (int)version;
    config.m_pUserFontPaths = &font;
    config.m_pIsolate = nullptr;
    config.m_v8EmbedderSlot = 0;
    FPDF_InitLibraryWithConfig(&config);
}

void DP_DestroyLibrary(Dart_NativeArguments arguments)
{
    FPDF_DestroyLibrary();
}

void DP_LoadDocument(Dart_NativeArguments arguments)
{
    NativeArgCString(filepath, 0, 1);
    NativeArgCString(password, 1, 0);
    Dart_Handle result = Dart_Null();
    void *doc = FPDF_LoadDocument(filepath, password);
    if (doc)
    {
        result = DP_NewObject();
        Dart_SetPeer(result, doc);
    }
    Dart_SetReturnValue(arguments, result);
}

void DP_CloseDocument(Dart_NativeArguments arguments)
{
    NativeArgPeer(doc, 0, 1);
    FPDF_CloseDocument((FPDF_DOCUMENT)doc);
}

void DP_GetFileVersion(Dart_NativeArguments arguments)
{
    NativeArgPeer(doc, 0, 1);
    int fileVersion = -1;
    FPDF_GetFileVersion((FPDF_DOCUMENT)doc, &fileVersion);
    Dart_SetReturnValue(arguments, Dart_NewInteger(fileVersion));
}

void DP_GetDocPermissions(Dart_NativeArguments arguments)
{
    NativeArgPeer(doc, 0, 1);
    uint64_t permission = FPDF_GetDocPermissions((FPDF_DOCUMENT)doc);
    Dart_SetReturnValue(arguments, Dart_NewInteger(permission));
}

void DP_GetSecurityHandlerRevision(Dart_NativeArguments arguments)
{
    NativeArgPeer(doc, 0, 1);
    int revision = FPDF_GetSecurityHandlerRevision((FPDF_DOCUMENT)doc);
    Dart_SetReturnValue(arguments, Dart_NewInteger(revision));
}

void DP_GetPageCount(Dart_NativeArguments arguments)
{
    NativeArgPeer(doc, 0, 1);
    int count = FPDF_GetPageCount((FPDF_DOCUMENT)doc);
    Dart_SetReturnValue(arguments, Dart_NewInteger(count));
}

void DP_LoadPage(Dart_NativeArguments arguments)
{
    NativeArgPeer(doc, 0, 1);
    NativeArgInt64(index, 1);
    Dart_Handle result = Dart_Null();
    void *page = FPDF_LoadPage((FPDF_DOCUMENT)doc, (int)index);
    if (page)
    {
        result = DP_NewObject();
        Dart_SetPeer(result, page);
    }
    Dart_SetReturnValue(arguments, result);
}

void DP_ClosePage(Dart_NativeArguments arguments)
{
    NativeArgPeer(page, 0, 1);
    FPDF_ClosePage((FPDF_PAGE)page);
}

void DP_GetPageWidth(Dart_NativeArguments arguments)
{
    NativeArgPeer(page, 0, 1);
    double width = FPDF_GetPageWidth((FPDF_PAGE)page);
    Dart_SetReturnValue(arguments, Dart_NewDouble(width));
}

void DP_GetPageHeight(Dart_NativeArguments arguments)
{
    NativeArgPeer(page, 0, 1);
    double height = FPDF_GetPageHeight((FPDF_PAGE)page);
    Dart_SetReturnValue(arguments, Dart_NewDouble(height));
}

void DP_GetPageBoundingBox(Dart_NativeArguments arguments)
{
    NativeArgPeer(page, 0, 1);
    FS_RECTF rect;
    bool suc = FPDF_GetPageBoundingBox((FPDF_PAGE)page, &rect);
    Dart_Handle result = Dart_Null();
    if (suc)
    {
        result = Dart_NewList(4);
        Dart_ListSetAt(result, 0, Dart_NewDouble(rect.left));
        Dart_ListSetAt(result, 1, Dart_NewDouble(rect.right));
        Dart_ListSetAt(result, 2, Dart_NewDouble(rect.top));
        Dart_ListSetAt(result, 3, Dart_NewDouble(rect.bottom));
    }
    Dart_SetReturnValue(arguments, result);
}

void DP_Bitmap_Create(Dart_NativeArguments arguments)
{
    NativeArgInt64(width, 0);
    NativeArgInt64(height, 1);
    NativeArgInt64(alpha, 2);
    Dart_Handle result = Dart_Null();
    void *bitmap = FPDFBitmap_Create((int)width, (int)height, (int)alpha);
    if (bitmap)
    {
        result = DP_NewObject();
        Dart_SetPeer(result, bitmap);
    }
    Dart_SetReturnValue(arguments, result);
}

// void DP_Bitmap_CreateEx(Dart_NativeArguments arguments)
// {
//     NativeArgInt64(width, 0);
//     NativeArgInt64(height, 1);
//     NativeArgInt64(format, 2);
//     NativeArgPeer(first_scan, 3, 1);
//     NativeArgInt64(stride, 4);
//     Dart_Handle result = Dart_Null();
//     void *bitmap = FPDFBitmap_CreateEx((int)width, (int)height, (int)format, first_scan, (int)stride);
//     if (bitmap)
//     {
//         result = DP_NewObject();
//         Dart_SetPeer(result, bitmap);
//     }
//     Dart_SetReturnValue(arguments, result);
// }

void DP_Bitmap_GetFormat(Dart_NativeArguments arguments)
{
    NativeArgPeer(bitmap, 0, 1);
    int format = FPDFBitmap_GetFormat((FPDF_BITMAP)bitmap);
    Dart_SetReturnValue(arguments, Dart_NewInteger(format));
}

void DP_Bitmap_FillRect(Dart_NativeArguments arguments)
{
    NativeArgPeer(bitmap, 0, 1);
    NativeArgInt64(left, 1);
    NativeArgInt64(top, 2);
    NativeArgInt64(width, 3);
    NativeArgInt64(height, 4);
    NativeArgInt64(color, 5);
    FPDFBitmap_FillRect((FPDF_BITMAP)bitmap, (int)left, (int)top, (int)width, (int)height, (FPDF_DWORD)color);
}

void DP_Bitmap_GetBuffer(Dart_NativeArguments arguments)
{
    NativeArgPeer(bitmap, 0, 1);
    void *buffer = FPDFBitmap_GetBuffer((FPDF_BITMAP)bitmap);
    Dart_Handle result = DP_NewObject();
    Dart_SetPeer(result, buffer);
    Dart_SetReturnValue(arguments, result);
}

void DP_Bitmap_GetWidth(Dart_NativeArguments arguments)
{
    NativeArgPeer(bitmap, 0, 1);
    int width = FPDFBitmap_GetWidth((FPDF_BITMAP)bitmap);
    Dart_SetReturnValue(arguments, Dart_NewInteger(width));
}

void DP_Bitmap_GetHeight(Dart_NativeArguments arguments)
{
    NativeArgPeer(bitmap, 0, 1);
    int height = FPDFBitmap_GetHeight((FPDF_BITMAP)bitmap);
    Dart_SetReturnValue(arguments, Dart_NewInteger(height));
}

void DP_Bitmap_GetStride(Dart_NativeArguments arguments)
{
    NativeArgPeer(bitmap, 0, 1);
    int stride = FPDFBitmap_GetStride((FPDF_BITMAP)bitmap);
    Dart_SetReturnValue(arguments, Dart_NewInteger(stride));
}

void DP_Bitmap_Destroy(Dart_NativeArguments arguments)
{
    NativeArgPeer(bitmap, 0, 1);
    FPDFBitmap_Destroy((FPDF_BITMAP)bitmap);
}

void DP_RenderPageBitmap(Dart_NativeArguments arguments)
{
    NativeArgPeer(bitmap, 0, 1);
    NativeArgPeer(page, 1, 1);
    NativeArgInt64(start_x, 2);
    NativeArgInt64(start_y, 3);
    NativeArgInt64(size_x, 4);
    NativeArgInt64(size_y, 5);
    NativeArgInt64(rotate, 6);
    NativeArgInt64(flags, 7);
    FPDF_RenderPageBitmap((FPDF_BITMAP)bitmap, (FPDF_PAGE)page, start_x, start_y, size_x, size_y, rotate, flags);
}

void DP_GetThumbnailAsBitmap(Dart_NativeArguments arguments)
{
    NativeArgPeer(page, 0, 1);
    void *bitmap = FPDFPage_GetThumbnailAsBitmap((FPDF_PAGE)page);
    Dart_Handle result = Dart_Null();
    if (bitmap)
    {
        result = DP_NewObject();
        Dart_SetPeer(result, bitmap);
    }
    Dart_SetReturnValue(arguments, result);
}

void DP_Text_LoadPage(Dart_NativeArguments arguments)
{
    NativeArgPeer(page, 0, 1);
    Dart_Handle result = Dart_Null();
    void *text = FPDFText_LoadPage((FPDF_PAGE)page);
    if (text)
    {
        result = DP_NewObject();
        Dart_SetPeer(result, text);
    }
    Dart_SetReturnValue(arguments, result);
}

void DP_Text_ClosePage(Dart_NativeArguments arguments)
{
    NativeArgPeer(text, 0, 1);
    FPDFText_ClosePage((FPDF_TEXTPAGE)text);
}

void DP_Text_CountChars(Dart_NativeArguments arguments)
{
    NativeArgPeer(text, 0, 1);
    int count = FPDFText_CountChars((FPDF_TEXTPAGE)text);
    Dart_SetReturnValue(arguments, Dart_NewInteger(count));
}

void DP_Text_GetUnicode(Dart_NativeArguments arguments)
{
    NativeArgPeer(text, 0, 1);
    NativeArgInt64(index, 1);
    unsigned int unicode = FPDFText_GetUnicode((FPDF_TEXTPAGE)text, (int)index);
    Dart_SetReturnValue(arguments, Dart_NewIntegerFromUint64(unicode));
}

void DP_Text_GetFontSize(Dart_NativeArguments arguments)
{
    NativeArgPeer(text, 0, 1);
    NativeArgInt64(index, 1);
    double fontSize = FPDFText_GetFontSize((FPDF_TEXTPAGE)text, (int)index);
    Dart_SetReturnValue(arguments, Dart_NewDouble(fontSize));
}

void DP_Text_GetFontInfo(Dart_NativeArguments arguments)
{
    NativeArgPeer(text, 0, 1);
    NativeArgInt64(index, 1);
    int flags;
    char buffer[1025];
    unsigned long l = FPDFText_GetFontInfo((FPDF_TEXTPAGE)text, (int)index, buffer, 1024, &flags);
    buffer[l] = 0;
    Dart_SetReturnValue(arguments, Dart_NewStringFromCString(buffer));
}

void DP_Text_GetCharAngle(Dart_NativeArguments arguments)
{
    NativeArgPeer(text, 0, 1);
    NativeArgInt64(index, 1);
    double angle = FPDFText_GetCharAngle((FPDF_TEXTPAGE)text, (int)index);
    Dart_SetReturnValue(arguments, Dart_NewDouble(angle));
}

void DP_Text_GetCharBox(Dart_NativeArguments arguments)
{
    NativeArgPeer(text, 0, 1);
    NativeArgInt64(index, 1);
    double left, right, bottom, top;
    FPDF_BOOL suc = FPDFText_GetCharBox((FPDF_TEXTPAGE)text, (int)index, &left, &right, &bottom, &top);
    Dart_Handle result = Dart_Null();
    if (suc)
    {
        result = Dart_NewList(4);
        Dart_ListSetAt(result, 0, Dart_NewDouble(left));
        Dart_ListSetAt(result, 1, Dart_NewDouble(right));
        Dart_ListSetAt(result, 2, Dart_NewDouble(top));
        Dart_ListSetAt(result, 3, Dart_NewDouble(bottom));
    }
    Dart_SetReturnValue(arguments, result);
}

void DP_Text_GetCharOrigin(Dart_NativeArguments arguments)
{
    NativeArgPeer(text, 0, 1);
    NativeArgInt64(index, 1);
    double x, y;
    FPDF_BOOL suc = FPDFText_GetCharOrigin((FPDF_TEXTPAGE)text, (int)index, &x, &y);
    Dart_Handle result = Dart_Null();
    if (suc)
    {
        result = Dart_NewList(2);
        Dart_ListSetAt(result, 0, Dart_NewDouble(x));
        Dart_ListSetAt(result, 1, Dart_NewDouble(y));
    }
    Dart_SetReturnValue(arguments, result);
}

void DP_Text_GetCharIndexAtPos(Dart_NativeArguments arguments)
{
    NativeArgPeer(text, 0, 1);
    NativeArgDouble(x, 1);
    NativeArgDouble(y, 2);
    NativeArgDouble(xTolerance, 3);
    NativeArgDouble(yTolerance, 4);
    int index = FPDFText_GetCharIndexAtPos((FPDF_TEXTPAGE)text, x, y, xTolerance, yTolerance);
    Dart_SetReturnValue(arguments, Dart_NewInteger(index));
}

void DP_Text_GetText(Dart_NativeArguments arguments)
{
    NativeArgPeer(text, 0, 1);
    NativeArgInt64(start_index, 1);
    NativeArgInt64(count, 2);
    uint16_t buffer[count + 1];
    int len = FPDFText_GetText((FPDF_TEXTPAGE)text, (int)start_index, (int)count, (unsigned short *)buffer);
    Dart_SetReturnValue(arguments, Dart_NewStringFromUTF16(buffer, len - 1));
}

void DP_Text_CountRects(Dart_NativeArguments arguments)
{
    NativeArgPeer(text, 0, 1);
    NativeArgInt64(start_index, 1);
    NativeArgInt64(count, 2);
    int len = FPDFText_CountRects((FPDF_TEXTPAGE)text, (int)start_index, (int)count);
    Dart_SetReturnValue(arguments, Dart_NewInteger(len));
}

void DP_Text_GetRect(Dart_NativeArguments arguments)
{
    NativeArgPeer(text, 0, 1);
    NativeArgInt64(rect_index, 1);
    double left, right, bottom, top;
    FPDF_BOOL suc = FPDFText_GetRect((FPDF_TEXTPAGE)text, (int)rect_index, &left, &right, &bottom, &top);
    Dart_Handle result = Dart_Null();
    if (suc)
    {
        result = Dart_NewList(4);
        Dart_ListSetAt(result, 0, Dart_NewDouble(left));
        Dart_ListSetAt(result, 1, Dart_NewDouble(right));
        Dart_ListSetAt(result, 2, Dart_NewDouble(top));
        Dart_ListSetAt(result, 3, Dart_NewDouble(bottom));
    }
    Dart_SetReturnValue(arguments, result);
}

void DP_Text_GetBoundedText(Dart_NativeArguments arguments)
{
    NativeArgPeer(text, 0, 1);
    NativeArgDouble(left, 1);
    NativeArgDouble(right, 2);
    NativeArgDouble(bottom, 3);
    NativeArgDouble(top, 4);
    uint16_t buffer[1024];
    int len = FPDFText_GetBoundedText((FPDF_TEXTPAGE)text, left, right, bottom, top, (unsigned short *)buffer, 1024);
    Dart_SetReturnValue(arguments, Dart_NewStringFromUTF16(buffer, len - 1));
}

void DP_Text_FindStart(Dart_NativeArguments arguments)
{
    NativeArgPeer(text, 0, 1);
    NativeArgUTF16String(findwhat, 1);
    NativeArgInt64(flags, 2);
    NativeArgInt64(start_index, 3);
    void *handle = FPDFText_FindStart((FPDF_TEXTPAGE)text, (FPDF_WIDESTRING)findwhat, flags, start_index);
    Dart_Handle result = Dart_Null();
    if (handle)
    {
        result = DP_NewObject();
        Dart_SetPeer(result, handle);
    }
    Dart_SetReturnValue(arguments, result);
}

void DP_Text_FindNext(Dart_NativeArguments arguments)
{
    NativeArgPeer(handle, 0, 1);
    bool having = FPDFText_FindNext((FPDF_SCHHANDLE)handle);
    Dart_SetReturnValue(arguments, Dart_NewBoolean(having));
}

void DP_Text_FindPrev(Dart_NativeArguments arguments)
{
    NativeArgPeer(handle, 0, 1);
    bool having = FPDFText_FindPrev((FPDF_SCHHANDLE)handle);
    Dart_SetReturnValue(arguments, Dart_NewBoolean(having));
}

void DP_Text_GetSchResultIndex(Dart_NativeArguments arguments)
{
    NativeArgPeer(handle, 0, 1);
    int index = FPDFText_GetSchResultIndex((FPDF_SCHHANDLE)handle);
    Dart_SetReturnValue(arguments, Dart_NewInteger(index));
}

void DP_Text_GetSchCount(Dart_NativeArguments arguments)
{
    NativeArgPeer(handle, 0, 1);
    int count = FPDFText_GetSchCount((FPDF_SCHHANDLE)handle);
    Dart_SetReturnValue(arguments, Dart_NewInteger(count));
}

void DP_Text_FindClose(Dart_NativeArguments arguments)
{
    NativeArgPeer(handle, 0, 1);
    FPDFText_FindClose((FPDF_SCHHANDLE)handle);
}

void DP_Bookmark_GetFirstChild(Dart_NativeArguments arguments)
{
    NativeArgPeer(doc, 0, 1);
    NativeArgPeer(bookmark, 1, 0);
    FPDF_BOOKMARK child = FPDFBookmark_GetFirstChild((FPDF_DOCUMENT)doc, (FPDF_BOOKMARK)bookmark);
    Dart_Handle result = Dart_Null();
    if (child)
    {
        result = DP_NewObject();
        Dart_SetPeer(result, child);
    }
    Dart_SetReturnValue(arguments, result);
}

void DP_Bookmark_GetNextSibling(Dart_NativeArguments arguments)
{
    NativeArgPeer(doc, 0, 1);
    NativeArgPeer(bookmark, 1, 1);
    FPDF_BOOKMARK next = FPDFBookmark_GetNextSibling((FPDF_DOCUMENT)doc, (FPDF_BOOKMARK)bookmark);
    Dart_Handle result = Dart_Null();
    if (next)
    {
        result = DP_NewObject();
        Dart_SetPeer(result, next);
    }
    Dart_SetReturnValue(arguments, result);
}

void DP_Bookmark_GetTitle(Dart_NativeArguments arguments)
{
    NativeArgPeer(bookmark, 0, 1);
    uint16_t buffer[1024];
    unsigned long len = FPDFBookmark_GetTitle((FPDF_BOOKMARK)bookmark, buffer, 1024);
    Dart_SetReturnValue(arguments, Dart_NewStringFromUTF16(buffer, len / 2 - 1));
}

// void DP_Bookmark_Find(Dart_NativeArguments arguments)
// {
//     NativeArgPeer(doc, 0, 1);
//     NativeArgUTF16String(title, 1);
//     FPDF_BOOKMARK bookmark = FPDFBookmark_Find((FPDF_DOCUMENT)doc, (FPDF_WIDESTRING)title);
//     Dart_Handle result = Dart_Null();
//     if (bookmark)
//     {
//         result = DP_NewObject();
//         Dart_SetPeer(result, bookmark);
//     }
//     Dart_SetReturnValue(arguments, result);
// }

void DP_Bookmark_GetDest(Dart_NativeArguments arguments)
{
    NativeArgPeer(doc, 0, 1);
    NativeArgPeer(bookmark, 1, 1);
    FPDF_DEST dest = FPDFBookmark_GetDest((FPDF_DOCUMENT)doc, (FPDF_BOOKMARK)bookmark);
    Dart_Handle result = Dart_Null();
    if (dest)
    {
        result = DP_NewObject();
        Dart_SetPeer(result, dest);
    }
    Dart_SetReturnValue(arguments, result);
}

void DP_Dest_GetDestPageIndex(Dart_NativeArguments arguments)
{
    NativeArgPeer(doc, 0, 1);
    NativeArgPeer(dest, 1, 1);
    int index = FPDFDest_GetDestPageIndex((FPDF_DOCUMENT)doc, (FPDF_DEST)dest);
    Dart_SetReturnValue(arguments, Dart_NewInteger(index));
}

Dart_NativeFunction DP_ResolveName(Dart_Handle name, int argc, bool *auto_setup_scope)
{
    if (!Dart_IsString(name))
    {
        return NULL;
    }
    const char *cname;
    Dart_NativeFunction result = NULL;
    if (DP_CheckError(Dart_StringToCString(name, &cname)))
    {
        return NULL;
    }
    if (strcmp("DP_LastError", cname) == 0)
        result = DP_LastError;
    if (strcmp("DP_InitLibrary", cname) == 0)
        result = DP_InitLibrary;
    else if (strcmp("DP_InitLibraryWithConfig", cname) == 0)
        result = DP_InitLibraryWithConfig;
    else if (strcmp("DP_DestroyLibrary", cname) == 0)
        result = DP_DestroyLibrary;
    else if (strcmp("DP_LoadDocument", cname) == 0)
        result = DP_LoadDocument;
    else if (strcmp("DP_CloseDocument", cname) == 0)
        result = DP_CloseDocument;
    else if (strcmp("DP_GetFileVersion", cname) == 0)
        result = DP_GetFileVersion;
    else if (strcmp("DP_GetDocPermissions", cname) == 0)
        result = DP_GetDocPermissions;
    else if (strcmp("DP_GetSecurityHandlerRevision", cname) == 0)
        result = DP_GetSecurityHandlerRevision;
    else if (strcmp("DP_GetPageCount", cname) == 0)
        result = DP_GetPageCount;
    else if (strcmp("DP_LoadPage", cname) == 0)
        result = DP_LoadPage;
    else if (strcmp("DP_ClosePage", cname) == 0)
        result = DP_ClosePage;
    else if (strcmp("DP_GetPageWidth", cname) == 0)
        result = DP_GetPageWidth;
    else if (strcmp("DP_GetPageHeight", cname) == 0)
        result = DP_GetPageHeight;
    else if (strcmp("DP_GetPageBoundingBox", cname) == 0)
        result = DP_GetPageBoundingBox;
    else if (strcmp("DP_Bitmap_Create", cname) == 0)
        result = DP_Bitmap_Create;
    // else if (strcmp("DP_Bitmap_CreateEx", cname) == 0)
    //     result = DP_Bitmap_CreateEx;
    else if (strcmp("DP_Bitmap_GetFormat", cname) == 0)
        result = DP_Bitmap_GetFormat;
    else if (strcmp("DP_Bitmap_FillRect", cname) == 0)
        result = DP_Bitmap_FillRect;
    else if (strcmp("DP_Bitmap_GetBuffer", cname) == 0)
        result = DP_Bitmap_GetBuffer;
    else if (strcmp("DP_Bitmap_GetWidth", cname) == 0)
        result = DP_Bitmap_GetWidth;
    else if (strcmp("DP_Bitmap_GetHeight", cname) == 0)
        result = DP_Bitmap_GetHeight;
    else if (strcmp("DP_Bitmap_GetStride", cname) == 0)
        result = DP_Bitmap_GetStride;
    else if (strcmp("DP_Bitmap_Destroy", cname) == 0)
        result = DP_Bitmap_Destroy;
    else if (strcmp("DP_RenderPageBitmap", cname) == 0)
        result = DP_RenderPageBitmap;
    else if (strcmp("DP_GetThumbnailAsBitmap", cname) == 0)
        result = DP_GetThumbnailAsBitmap;
    else if (strcmp("DP_Text_LoadPage", cname) == 0)
        result = DP_Text_LoadPage;
    else if (strcmp("DP_Text_ClosePage", cname) == 0)
        result = DP_Text_ClosePage;
    else if (strcmp("DP_Text_CountChars", cname) == 0)
        result = DP_Text_CountChars;
    else if (strcmp("DP_Text_GetUnicode", cname) == 0)
        result = DP_Text_GetUnicode;
    else if (strcmp("DP_Text_GetFontSize", cname) == 0)
        result = DP_Text_GetFontSize;
    else if (strcmp("DP_Text_GetFontInfo", cname) == 0)
        result = DP_Text_GetFontInfo;
    else if (strcmp("DP_Text_GetCharAngle", cname) == 0)
        result = DP_Text_GetCharAngle;
    else if (strcmp("DP_Text_GetCharBox", cname) == 0)
        result = DP_Text_GetCharBox;
    else if (strcmp("DP_Text_GetCharOrigin", cname) == 0)
        result = DP_Text_GetCharOrigin;
    else if (strcmp("DP_Text_GetCharIndexAtPos", cname) == 0)
        result = DP_Text_GetCharIndexAtPos;
    else if (strcmp("DP_Text_GetText", cname) == 0)
        result = DP_Text_GetText;
    else if (strcmp("DP_Text_CountRects", cname) == 0)
        result = DP_Text_CountRects;
    else if (strcmp("DP_Text_GetRect", cname) == 0)
        result = DP_Text_GetRect;
    else if (strcmp("DP_Text_GetBoundedText", cname) == 0)
        result = DP_Text_GetBoundedText;
    else if (strcmp("DP_Text_FindStart", cname) == 0)
        result = DP_Text_FindStart;
    else if (strcmp("DP_Text_FindNext", cname) == 0)
        result = DP_Text_FindNext;
    else if (strcmp("DP_Text_FindPrev", cname) == 0)
        result = DP_Text_FindPrev;
    else if (strcmp("DP_Text_GetSchResultIndex", cname) == 0)
        result = DP_Text_GetSchResultIndex;
    else if (strcmp("DP_Text_GetSchCount", cname) == 0)
        result = DP_Text_GetSchCount;
    else if (strcmp("DP_Text_FindClose", cname) == 0)
        result = DP_Text_FindClose;
    else if (strcmp("DP_Bookmark_GetFirstChild", cname) == 0)
        result = DP_Bookmark_GetFirstChild;
    else if (strcmp("DP_Bookmark_GetNextSibling", cname) == 0)
        result = DP_Bookmark_GetNextSibling;
    else if (strcmp("DP_Bookmark_GetTitle", cname) == 0)
        result = DP_Bookmark_GetTitle;
    // else if (strcmp("DP_Bookmark_Find", cname) == 0)
    //     result = DP_Bookmark_Find;
    else if (strcmp("DP_Bookmark_GetDest", cname) == 0)
        result = DP_Bookmark_GetDest;
    else if (strcmp("DP_Dest_GetDestPageIndex", cname) == 0)
        result = DP_Dest_GetDestPageIndex;
    return result;
}
