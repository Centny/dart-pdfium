library dart_pdfium;

import 'dart-ext:dart_pdfium';

void InitLibrary() native "DP_InitLibrary";
void DestroyLibrary() native "DP_DestroyLibrary";
Object _nativeLoadDocument(String filepath, password) native "DP_LoadDocument";
void _nativeCloseDocument(Object doc) native "DP_CloseDocument";
int _nativeGetFileVersion(Object doc) native "DP_GetFileVersion";
int _nativeGetDocPermissions(Object doc) native "DP_GetDocPermissions";
int _nativeGetSecurityHandlerRevision(Object doc) native "DP_GetSecurityHandlerRevision";
int _nativeGetPageCount(Object doc) native "DP_GetPageCount";
Object _nativeLoadPage(Object doc, int index) native "DP_LoadPage";
void _nativeClosePage(Object page) native "DP_ClosePage";
double _nativeGetPageWidth(Object page) native "DP_GetPageWidth";
double _nativeGetPageHeight(Object page) native "DP_GetPageHeight";
List _nativeGetPageBoundingBox(Object page) native "DP_GetPageBoundingBox";
Object _nativeBitmap_Create(int width, height, alpha) native "DP_Bitmap_Create";
Object _nativeBitmap_CreateEx(int width, height, format, Object firstScan, int stride) native "DP_Bitmap_CreateEx";
int _nativeBitmap_GetFormat(Object bitmap) native "DP_Bitmap_GetFormat";
void _nativeBitmap_FillRect(Object bitmap, int left, top, width, height, color) native "DP_Bitmap_FillRect";
Object _nativeBitmap_GetBuffer(Object bitmap) native "DP_Bitmap_GetBuffer";
int _nativeBitmap_GetWidth(Object bitmap) native "DP_Bitmap_GetWidth";
int _nativeBitmap_GetHeight(Object bitmap) native "DP_Bitmap_GetHeight";
int _nativeBitmap_GetStride(Object bitmap) native "DP_Bitmap_GetStride";
int _nativeBitmap_Destroy(Object bitmap) native "DP_Bitmap_Destroy";
int _nativeRenderPageBitmap(Object bitmap, page, int start_x, start_y, size_x, size_y, rotate, flags) native "DP_RenderPageBitmap";

class Bitmap {
  Object _bitmap;
  Bitmap(int width, height, {int alpha = 1, int format = 0, Object firstScan = null, int stride = 0}) {
    if (firstScan == null) {
      _bitmap = _nativeBitmap_Create(width, height, alpha);
    } else {
      _bitmap = _nativeBitmap_CreateEx(width, height, format, firstScan, stride);
    }
  }
  Object get bitmap => _bitmap;
  int get format => _nativeBitmap_GetFormat(_bitmap);
  Object get buffer => _nativeBitmap_GetBuffer(_bitmap);
  int get width => _nativeBitmap_GetWidth(_bitmap);
  int get height => _nativeBitmap_GetHeight(_bitmap);
  int get stride => _nativeBitmap_GetStride(_bitmap);
  void FillRect(int left, top, width, height, color) => _nativeBitmap_FillRect(_bitmap, left, top, width, height, color);
  void Destory() => _nativeBitmap_Destroy(_bitmap);
}

class Rect {
  double left, right, top, bottom;
  String toString() => "Rect($left,$right,$top,$bottom)";
}

class Page {
  Object _page;
  Document _doc;
  int _index;
  Page(Document doc, int index) {
    _doc = doc;
    _index = index;
    _page = _nativeLoadPage(_doc.doc, index);
  }
  Object get page => _page;
  Document get document => _doc;
  int get index => _index;
  double get width => _nativeGetPageWidth(_page);
  double get height => _nativeGetPageHeight(_page);
  Rect get boundingBox {
    List res = _nativeGetPageBoundingBox(_page);
    bool suc = res[0];
    Rect rect = null;
    if (suc) {
      rect = new Rect();
      rect.left = res[1];
      rect.right = res[2];
      rect.top = res[3];
      rect.bottom = res[4];
    }
    return rect;
  }

  void Render(Bitmap bitmap, int start_x, start_y, size_x, size_y, rotate, flags) =>
      _nativeRenderPageBitmap(bitmap.bitmap, _page, start_x, start_y, size_x, size_y, rotate, flags);

  void Close() => _nativeClosePage(_page);

  String toString() => "Page(${_doc.filepath},$_index)";
}

class Document {
  Object _doc;
  String _filepath;
  //
  Document(String filepath, password) {
    _filepath = filepath;
    _doc = _nativeLoadDocument(filepath, password);
  }

  Object get doc => _doc;

  String get filepath => _filepath;

  int get version => _nativeGetFileVersion(_doc);

  int get docPermissions => _nativeGetDocPermissions(_doc);

  int get securityHandlerRevision => _nativeGetSecurityHandlerRevision(_doc);

  int get pageCount => _nativeGetPageCount(_doc);

  Page loadPage(int index) => new Page(this, index);

  void Close() => _nativeCloseDocument(_doc);

  String toString() => "Document($_filepath)";
}
