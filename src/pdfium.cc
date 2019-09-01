#include <string.h>
#include <stdio.h>
#include "dart_api.h"
#include "fpdfview.h"
// Forward declaration of ResolveName function.
Dart_NativeFunction DP_ResolveName(Dart_Handle name, int argc, bool *auto_setup_scope);

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

Dart_Handle DP_HandleError(Dart_Handle handle)
{
    if (Dart_IsError(handle))
    {
        Dart_PropagateError(handle);
    }
    return handle;
}

Dart_Handle DP_NewObject()
{
    Dart_Handle library = Dart_LookupLibrary(Dart_NewStringFromCString("dart:core"));
    Dart_Handle name = Dart_NewStringFromCString("Object");
    Dart_Handle type = Dart_GetType(library, name, 0, NULL);
    return Dart_New(type, Dart_Null(), 0, NULL);
}

Dart_Handle DP_LastError()
{
    Dart_Handle result = Dart_Null();
    unsigned long err = FPDF_GetLastError();
    switch (err)
    {
    case FPDF_ERR_SUCCESS:
        result = Dart_Null();
        break;
    case FPDF_ERR_UNKNOWN:
        result = Dart_NewApiError("Unknown error");
        break;
    case FPDF_ERR_FILE:
        result = Dart_NewApiError("File not found or could not be opened");
        break;
    case FPDF_ERR_FORMAT:
        result = Dart_NewApiError("File not in PDF format or corrupted");
        break;
    case FPDF_ERR_PASSWORD:
        result = Dart_NewApiError("Password required or incorrect password");
        break;
    case FPDF_ERR_SECURITY:
        result = Dart_NewApiError("Unsupported security scheme");
        break;
    case FPDF_ERR_PAGE:
        result = Dart_NewApiError("Page not found or content error");
        break;
    default:
        char buf[1024];
        int l = sprintf(buf, "Unknown error %ld", err);
        buf[l] = 0;
        result = Dart_NewApiError(buf);
    }
    return result;
}

void DP_InitLibrary(Dart_NativeArguments arguments)
{
    FPDF_InitLibrary();
}

void DP_InitLibraryWithConfig(Dart_NativeArguments arguments)
{
    Dart_Handle version_h = Dart_GetNativeArgument(arguments, 0);
    int64_t ver;
    if (DP_CheckError(Dart_IntegerToInt64(version_h, &ver)))
    {
        return;
    }
    Dart_Handle font_h = Dart_GetNativeArgument(arguments, 1);
    const char *font;
    if (DP_CheckError(Dart_StringToCString(font_h, &font)))
    {
        return;
    }
    FPDF_LIBRARY_CONFIG config;
    config.version = (int)ver;
    config.m_pUserFontPaths = &font;
    FPDF_InitLibraryWithConfig(&config);
}

void DP_DestroyLibrary(Dart_NativeArguments arguments)
{
    FPDF_DestroyLibrary();
}

void DP_LoadDocument(Dart_NativeArguments arguments)
{
    const char *filepath = NULL;
    Dart_Handle filepath_h = Dart_GetNativeArgument(arguments, 0);
    if (DP_CheckError(Dart_StringToCString(filepath_h, &filepath)))
    {
        return;
    }
    const char *password = NULL;
    Dart_Handle password_h = Dart_GetNativeArgument(arguments, 1);
    if (!Dart_IsNull(password_h) && DP_CheckError(Dart_StringToCString(password_h, &password)))
    {
        return;
    }
    void *doc = FPDF_LoadDocument(filepath, password);
    if (!doc)
    {
        Dart_SetReturnValue(arguments, DP_HandleError(DP_LastError()));
        return;
    }
    Dart_Handle result = DP_NewObject();
    Dart_SetPeer(result, doc);
    Dart_SetReturnValue(arguments, result);
}

void DP_CloseDocument(Dart_NativeArguments arguments)
{
    void *doc = NULL;
    Dart_Handle doc_h = Dart_GetNativeArgument(arguments, 0);
    if (DP_CheckError(Dart_GetPeer(doc_h, &doc)))
    {
        Dart_SetReturnValue(arguments, Dart_NewApiError("get native doc fail"));
        return;
    }
    FPDF_CloseDocument((FPDF_DOCUMENT)doc);
    Dart_SetPeer(doc_h, NULL);
}

void DP_GetFileVersion(Dart_NativeArguments arguments)
{
    void *doc = NULL;
    Dart_Handle doc_h = Dart_GetNativeArgument(arguments, 0);
    if (DP_CheckError(Dart_GetPeer(doc_h, &doc)))
    {
        Dart_SetReturnValue(arguments, Dart_NewApiError("get native doc fail"));
        return;
    }
    int fileVersion;
    if (!FPDF_GetFileVersion((FPDF_DOCUMENT)doc, &fileVersion))
    {
        Dart_SetReturnValue(arguments, DP_HandleError(DP_LastError()));
        return;
    }
    Dart_SetReturnValue(arguments, Dart_NewInteger(fileVersion));
}

void DP_GetDocPermissions(Dart_NativeArguments arguments)
{
    void *doc = NULL;
    Dart_Handle doc_h = Dart_GetNativeArgument(arguments, 0);
    if (DP_CheckError(Dart_GetPeer(doc_h, &doc)))
    {
        Dart_SetReturnValue(arguments, Dart_NewApiError("get native doc fail"));
        return;
    }
    uint64_t permission = FPDF_GetDocPermissions((FPDF_DOCUMENT)doc);
    Dart_SetReturnValue(arguments, Dart_NewInteger(permission));
}

void DP_GetSecurityHandlerRevision(Dart_NativeArguments arguments)
{
    void *doc = NULL;
    Dart_Handle doc_h = Dart_GetNativeArgument(arguments, 0);
    if (DP_CheckError(Dart_GetPeer(doc_h, &doc)))
    {
        Dart_SetReturnValue(arguments, Dart_NewApiError("get native doc fail"));
        return;
    }
    int revision = FPDF_GetSecurityHandlerRevision((FPDF_DOCUMENT)doc);
    Dart_SetReturnValue(arguments, Dart_NewInteger(revision));
}

void DP_GetPageCount(Dart_NativeArguments arguments)
{
    void *doc = NULL;
    Dart_Handle doc_h = Dart_GetNativeArgument(arguments, 0);
    if (DP_CheckError(Dart_GetPeer(doc_h, &doc)))
    {
        Dart_SetReturnValue(arguments, Dart_NewApiError("get native doc fail"));
        return;
    }
    int count = FPDF_GetPageCount((FPDF_DOCUMENT)doc);
    Dart_SetReturnValue(arguments, Dart_NewInteger(count));
}

void DP_LoadPage(Dart_NativeArguments arguments)
{
    void *doc = NULL;
    Dart_Handle doc_h = Dart_GetNativeArgument(arguments, 0);
    if (DP_CheckError(Dart_GetPeer(doc_h, &doc)))
    {
        Dart_SetReturnValue(arguments, Dart_NewApiError("get native doc fail"));
        return;
    }
    int64_t index = 0;
    Dart_Handle index_h = Dart_GetNativeArgument(arguments, 1);
    if (DP_CheckError(Dart_IntegerToInt64(index_h, &index)))
    {
        return;
    }
    void *page = FPDF_LoadPage((FPDF_DOCUMENT)doc, (int)index);
    Dart_Handle result = DP_NewObject();
    Dart_SetPeer(result, page);
    Dart_SetReturnValue(arguments, result);
}

void DP_ClosePage(Dart_NativeArguments arguments)
{
    void *page = NULL;
    Dart_Handle page_h = Dart_GetNativeArgument(arguments, 0);
    if (DP_CheckError(Dart_GetPeer(page_h, &page)))
    {
        Dart_SetReturnValue(arguments, Dart_NewApiError("get native page fail"));
        return;
    }
    FPDF_ClosePage((FPDF_PAGE)page);
}

void DP_GetPageWidth(Dart_NativeArguments arguments)
{
    void *page = NULL;
    Dart_Handle page_h = Dart_GetNativeArgument(arguments, 0);
    if (DP_CheckError(Dart_GetPeer(page_h, &page)))
    {
        Dart_SetReturnValue(arguments, Dart_NewApiError("get native page fail"));
        return;
    }
    double width = FPDF_GetPageWidth((FPDF_PAGE)page);
    Dart_SetReturnValue(arguments, Dart_NewDouble(width));
}

void DP_GetPageHeight(Dart_NativeArguments arguments)
{
    void *page = NULL;
    Dart_Handle page_h = Dart_GetNativeArgument(arguments, 0);
    if (DP_CheckError(Dart_GetPeer(page_h, &page)))
    {
        Dart_SetReturnValue(arguments, Dart_NewApiError("get native page fail"));
        return;
    }
    double height = FPDF_GetPageHeight((FPDF_PAGE)page);
    Dart_SetReturnValue(arguments, Dart_NewDouble(height));
}

void DP_GetPageBoundingBox(Dart_NativeArguments arguments)
{
    void *page = NULL;
    Dart_Handle page_h = Dart_GetNativeArgument(arguments, 0);
    if (DP_CheckError(Dart_GetPeer(page_h, &page)))
    {
        Dart_SetReturnValue(arguments, Dart_NewApiError("get native page fail"));
        return;
    }
    FS_RECTF rect;
    bool suc = FPDF_GetPageBoundingBox((FPDF_PAGE)page, &rect);
    Dart_Handle result = Dart_NewList(5);
    Dart_ListSetAt(result, 0, Dart_NewBoolean(suc));
    Dart_ListSetAt(result, 1, Dart_NewDouble(rect.left));
    Dart_ListSetAt(result, 2, Dart_NewDouble(rect.right));
    Dart_ListSetAt(result, 3, Dart_NewDouble(rect.top));
    Dart_ListSetAt(result, 4, Dart_NewDouble(rect.bottom));
    Dart_SetReturnValue(arguments, result);
}

void DP_Bitmap_Create(Dart_NativeArguments arguments)
{
    int64_t width = 0;
    Dart_Handle width_h = Dart_GetNativeArgument(arguments, 0);
    if (DP_CheckError(Dart_IntegerToInt64(width_h, &width)))
    {
        return;
    }
    int64_t height = 0;
    Dart_Handle height_h = Dart_GetNativeArgument(arguments, 1);
    if (DP_CheckError(Dart_IntegerToInt64(height_h, &height)))
    {
        return;
    }
    int64_t alpha = 0;
    Dart_Handle alpha_h = Dart_GetNativeArgument(arguments, 2);
    if (DP_CheckError(Dart_IntegerToInt64(alpha_h, &alpha)))
    {
        return;
    }
    void *bitmap = FPDFBitmap_Create((int)width, (int)height, (int)alpha);
    Dart_Handle result = DP_NewObject();
    Dart_SetPeer(result, bitmap);
    Dart_SetReturnValue(arguments, result);
}

void DP_Bitmap_CreateEx(Dart_NativeArguments arguments)
{
    int64_t width = 0;
    Dart_Handle width_h = Dart_GetNativeArgument(arguments, 0);
    if (DP_CheckError(Dart_IntegerToInt64(width_h, &width)))
    {
        return;
    }
    int64_t height = 0;
    Dart_Handle height_h = Dart_GetNativeArgument(arguments, 1);
    if (DP_CheckError(Dart_IntegerToInt64(height_h, &height)))
    {
        return;
    }
    int64_t format = 0;
    Dart_Handle format_h = Dart_GetNativeArgument(arguments, 2);
    if (DP_CheckError(Dart_IntegerToInt64(format_h, &format)))
    {
        return;
    }
    void *first_scan = NULL;
    Dart_Handle first_scan_h = Dart_GetNativeArgument(arguments, 3);
    if (DP_CheckError(Dart_GetPeer(first_scan_h, &first_scan)))
    {
        Dart_SetReturnValue(arguments, Dart_NewApiError("get native buffer fail"));
        return;
    }
    int64_t stride = 0;
    Dart_Handle stride_h = Dart_GetNativeArgument(arguments, 4);
    if (DP_CheckError(Dart_IntegerToInt64(stride_h, &stride)))
    {
        return;
    }
    void *bitmap = FPDFBitmap_CreateEx((int)width, (int)height, (int)format, first_scan, (int)stride);
    Dart_Handle result = DP_NewObject();
    Dart_SetPeer(result, bitmap);
    Dart_SetReturnValue(arguments, result);
}

void DP_Bitmap_GetFormat(Dart_NativeArguments arguments)
{
    void *bitmap = NULL;
    Dart_Handle bitmap_h = Dart_GetNativeArgument(arguments, 0);
    if (DP_CheckError(Dart_GetPeer(bitmap_h, &bitmap)))
    {
        Dart_SetReturnValue(arguments, Dart_NewApiError("get native bitmap fail"));
        return;
    }
    int format = FPDFBitmap_GetFormat((FPDF_BITMAP)bitmap);
    Dart_SetReturnValue(arguments, Dart_NewInteger(format));
}

void DP_Bitmap_FillRect(Dart_NativeArguments arguments)
{
    void *bitmap = NULL;
    Dart_Handle bitmap_h = Dart_GetNativeArgument(arguments, 0);
    if (DP_CheckError(Dart_GetPeer(bitmap_h, &bitmap)))
    {
        Dart_SetReturnValue(arguments, Dart_NewApiError("get native bitmap fail"));
        return;
    }
    int64_t left = 0;
    Dart_Handle left_h = Dart_GetNativeArgument(arguments, 1);
    if (DP_CheckError(Dart_IntegerToInt64(left_h, &left)))
    {
        return;
    }
    int64_t top = 0;
    Dart_Handle top_h = Dart_GetNativeArgument(arguments, 2);
    if (DP_CheckError(Dart_IntegerToInt64(top_h, &top)))
    {
        return;
    }
    int64_t width = 0;
    Dart_Handle width_h = Dart_GetNativeArgument(arguments, 3);
    if (DP_CheckError(Dart_IntegerToInt64(width_h, &width)))
    {
        return;
    }
    int64_t height = 0;
    Dart_Handle height_h = Dart_GetNativeArgument(arguments, 4);
    if (DP_CheckError(Dart_IntegerToInt64(height_h, &height)))
    {
        return;
    }
    uint64_t color = 0;
    Dart_Handle color_h = Dart_GetNativeArgument(arguments, 5);
    if (DP_CheckError(Dart_IntegerToUint64(color_h, &color)))
    {
        return;
    }
    FPDFBitmap_FillRect((FPDF_BITMAP)bitmap, (int)left, (int)top, (int)width, (int)height, (FPDF_DWORD)color);
}

void DP_Bitmap_GetBuffer(Dart_NativeArguments arguments)
{
    void *bitmap = NULL;
    Dart_Handle bitmap_h = Dart_GetNativeArgument(arguments, 0);
    if (DP_CheckError(Dart_GetPeer(bitmap_h, &bitmap)))
    {
        Dart_SetReturnValue(arguments, Dart_NewApiError("get native bitmap fail"));
        return;
    }
    void *buffer = FPDFBitmap_GetBuffer((FPDF_BITMAP)bitmap);
    Dart_Handle result = DP_NewObject();
    Dart_SetPeer(result, buffer);
    Dart_SetReturnValue(arguments, result);
}

void DP_Bitmap_GetWidth(Dart_NativeArguments arguments)
{
    void *bitmap = NULL;
    Dart_Handle bitmap_h = Dart_GetNativeArgument(arguments, 0);
    if (DP_CheckError(Dart_GetPeer(bitmap_h, &bitmap)))
    {
        Dart_SetReturnValue(arguments, Dart_NewApiError("get native bitmap fail"));
        return;
    }
    int width = FPDFBitmap_GetWidth((FPDF_BITMAP)bitmap);
    Dart_SetReturnValue(arguments, Dart_NewInteger(width));
}

void DP_Bitmap_GetHeight(Dart_NativeArguments arguments)
{
    void *bitmap = NULL;
    Dart_Handle bitmap_h = Dart_GetNativeArgument(arguments, 0);
    if (DP_CheckError(Dart_GetPeer(bitmap_h, &bitmap)))
    {
        Dart_SetReturnValue(arguments, Dart_NewApiError("get native bitmap fail"));
        return;
    }
    int height = FPDFBitmap_GetHeight((FPDF_BITMAP)bitmap);
    Dart_SetReturnValue(arguments, Dart_NewInteger(height));
}

void DP_Bitmap_GetStride(Dart_NativeArguments arguments)
{
    void *bitmap = NULL;
    Dart_Handle bitmap_h = Dart_GetNativeArgument(arguments, 0);
    if (DP_CheckError(Dart_GetPeer(bitmap_h, &bitmap)))
    {
        Dart_SetReturnValue(arguments, Dart_NewApiError("get native bitmap fail"));
        return;
    }
    int stride = FPDFBitmap_GetStride((FPDF_BITMAP)bitmap);
    Dart_SetReturnValue(arguments, Dart_NewInteger(stride));
}

void DP_Bitmap_Destroy(Dart_NativeArguments arguments)
{
    void *bitmap = NULL;
    Dart_Handle bitmap_h = Dart_GetNativeArgument(arguments, 0);
    if (DP_CheckError(Dart_GetPeer(bitmap_h, &bitmap)))
    {
        Dart_SetReturnValue(arguments, Dart_NewApiError("get native bitmap fail"));
        return;
    }
    FPDFBitmap_Destroy((FPDF_BITMAP)bitmap);
}

void DP_RenderPageBitmap(Dart_NativeArguments arguments)
{
    void *bitmap = NULL;
    Dart_Handle bitmap_h = Dart_GetNativeArgument(arguments, 0);
    if (DP_CheckError(Dart_GetPeer(bitmap_h, &bitmap)))
    {
        Dart_SetReturnValue(arguments, Dart_NewApiError("get native bitmap fail"));
        return;
    }
    void *page = NULL;
    Dart_Handle page_h = Dart_GetNativeArgument(arguments, 1);
    if (DP_CheckError(Dart_GetPeer(page_h, &page)))
    {
        Dart_SetReturnValue(arguments, Dart_NewApiError("get native page fail"));
        return;
    }
    int64_t start_x = 0;
    Dart_Handle start_x_h = Dart_GetNativeArgument(arguments, 2);
    if (DP_CheckError(Dart_IntegerToInt64(start_x_h, &start_x)))
    {
        return;
    }
    int64_t start_y = 0;
    Dart_Handle start_y_h = Dart_GetNativeArgument(arguments, 3);
    if (DP_CheckError(Dart_IntegerToInt64(start_y_h, &start_y)))
    {
        return;
    }
    int64_t size_x = 0;
    Dart_Handle size_x_h = Dart_GetNativeArgument(arguments, 4);
    if (DP_CheckError(Dart_IntegerToInt64(size_x_h, &size_x)))
    {
        return;
    }
    int64_t size_y = 0;
    Dart_Handle size_y_h = Dart_GetNativeArgument(arguments, 5);
    if (DP_CheckError(Dart_IntegerToInt64(size_y_h, &size_y)))
    {
        return;
    }
    int64_t rotate = 0;
    Dart_Handle rotate_h = Dart_GetNativeArgument(arguments, 6);
    if (DP_CheckError(Dart_IntegerToInt64(rotate_h, &rotate)))
    {
        return;
    }
    int64_t flags = 0;
    Dart_Handle flags_h = Dart_GetNativeArgument(arguments, 7);
    if (DP_CheckError(Dart_IntegerToInt64(flags_h, &flags)))
    {
        return;
    }
    FPDF_RenderPageBitmap((FPDF_BITMAP)bitmap, (FPDF_PAGE)page, start_x, start_y, size_x, size_y, rotate, flags);
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
    else if (strcmp("DP_Bitmap_CreateEx", cname) == 0)
        result = DP_Bitmap_CreateEx;
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
    return result;
}
