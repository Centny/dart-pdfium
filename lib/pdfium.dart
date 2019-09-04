library pdfium;

import 'dart-ext:dart_pdfium';

void InitLibrary() native "DP_InitLibrary";
void InitLibraryWithConfig(int version, String font) native "DP_InitLibraryWithConfig";
void DestroyLibrary() native "DP_DestroyLibrary";
int DP_LastError() native "DP_LastError";
Object DP_LoadDocument(String filepath, password) native "DP_LoadDocument";
void DP_CloseDocument(Object doc) native "DP_CloseDocument";
int DP_GetFileVersion(Object doc) native "DP_GetFileVersion";
int DP_GetDocPermissions(Object doc) native "DP_GetDocPermissions";
int DP_GetSecurityHandlerRevision(Object doc) native "DP_GetSecurityHandlerRevision";
int DP_GetPageCount(Object doc) native "DP_GetPageCount";
Object DP_LoadPage(Object doc, int index) native "DP_LoadPage";
void DP_ClosePage(Object page) native "DP_ClosePage";
double DP_GetPageWidth(Object page) native "DP_GetPageWidth";
double DP_GetPageHeight(Object page) native "DP_GetPageHeight";
List DP_GetPageBoundingBox(Object page) native "DP_GetPageBoundingBox";
Object DP_Bitmap_Create(int width, height, alpha) native "DP_Bitmap_Create";
// Object DP_Bitmap_CreateEx(int width, height, format, Object firstScan, int stride) native "DP_Bitmap_CreateEx";
int DP_Bitmap_GetFormat(Object bitmap) native "DP_Bitmap_GetFormat";
void DP_Bitmap_FillRect(Object bitmap, int left, top, width, height, color) native "DP_Bitmap_FillRect";
Object DP_Bitmap_GetBuffer(Object bitmap) native "DP_Bitmap_GetBuffer";
int DP_Bitmap_GetWidth(Object bitmap) native "DP_Bitmap_GetWidth";
int DP_Bitmap_GetHeight(Object bitmap) native "DP_Bitmap_GetHeight";
int DP_Bitmap_GetStride(Object bitmap) native "DP_Bitmap_GetStride";
int DP_Bitmap_Destroy(Object bitmap) native "DP_Bitmap_Destroy";
int DP_RenderPageBitmap(Object bitmap, page, int start_x, start_y, size_x, size_y, rotate, flags) native "DP_RenderPageBitmap";
Object DP_GetThumbnailAsBitmap(Object page) native "DP_GetThumbnailAsBitmap";
Object DP_Text_LoadPage(Object page) native "DP_Text_LoadPage";
void DP_Text_ClosePage(Object text) native "DP_Text_ClosePage";
int DP_Text_CountChars(Object text) native "DP_Text_CountChars";
int DP_Text_GetUnicode(Object text, int index) native "DP_Text_GetUnicode";
double DP_Text_GetFontSize(Object text, int index) native "DP_Text_GetFontSize";
String DP_Text_GetFontInfo(Object text, int index) native "DP_Text_GetFontInfo";
double DP_Text_GetCharAngle(Object text, int index) native "DP_Text_GetCharAngle";
List DP_Text_GetCharBox(Object text, int index) native "DP_Text_GetCharBox";
List DP_Text_GetCharOrigin(Object text, int index) native "DP_Text_GetCharOrigin";
int DP_Text_GetCharIndexAtPos(Object text, double x, y, xTolerance, yTolerance) native "DP_Text_GetCharIndexAtPos";
String DP_Text_GetText(Object text, int start_index, count) native "DP_Text_GetText";
int DP_Text_CountRects(Object text, int start_index, count) native "DP_Text_CountRects";
List DP_Text_GetRect(Object text, int rect_index) native "DP_Text_GetRect";
String DP_Text_GetBoundedText(Object text, double left, right, bottom, top) native "DP_Text_GetBoundedText";
Object DP_Text_FindStart(Object text, findwhat, int flags, start_index) native "DP_Text_FindStart";
bool DP_Text_FindNext(Object handle) native "DP_Text_FindNext";
bool DP_Text_FindPrev(Object handle) native "DP_Text_FindPrev";
int DP_Text_GetSchResultIndex(Object handle) native "DP_Text_GetSchResultIndex";
int DP_Text_GetSchCount(Object handle) native "DP_Text_GetSchCount";
void DP_Text_FindClose(Object handle) native "DP_Text_FindClose";
Object DP_Bookmark_GetFirstChild(Object doc, bookmark) native "DP_Bookmark_GetFirstChild";
Object DP_Bookmark_GetNextSibling(Object doc, bookmark) native "DP_Bookmark_GetNextSibling";
String DP_Bookmark_GetTitle(Object bookmark) native "DP_Bookmark_GetTitle";
// Object DP_Bookmark_Find(Object bookmark, String title) native "DP_Bookmark_Find";
Object DP_Bookmark_GetDest(Object doc, bookmark) native "DP_Bookmark_GetDest";
int DP_Dest_GetDestPageIndex(Object doc, dest) native "DP_Dest_GetDestPageIndex";

class Rect {
  double left, right, top, bottom;
  String toString() => "Rect($left,$right,$top,$bottom)";
}

class Point {
  double x, y;
  String toString() => "Point($x,$y)";
}

class Bookmark {
  Object _bookmark;
  Document _doc;
  Object get nativeObject => _bookmark;
  Document get doc => _doc;
  Bookmark get next {
    Object bk = DP_Bookmark_GetNextSibling(_doc._doc, _bookmark);
    if (bk == null) {
      return null;
    }
    Bookmark bookmark = new Bookmark();
    bookmark._doc = _doc;
    bookmark._bookmark = bk;
    return bookmark;
  }

  Bookmark get firstChild {
    Object bk = DP_Bookmark_GetFirstChild(_doc._doc, _bookmark);
    if (bk == null) {
      return null;
    }
    Bookmark bookmark = new Bookmark();
    bookmark._doc = _doc;
    bookmark._bookmark = bk;
    return bookmark;
  }

  String get title => DP_Bookmark_GetTitle(_bookmark);
  int get pageIndex {
    Object d = DP_Bookmark_GetDest(_doc._doc, _bookmark);
    if (d == null) {
      return -1;
    }
    return DP_Dest_GetDestPageIndex(doc._doc, d);
  }

  String toString() => "Bookmark(${_doc._filepath})";
}

class Finder {
  Object _finder;
  PageText _text;
  String _findwhat;
  int _flags;
  int _startIndex;
  Object get nativeObject => _finder;
  PageText get text => _text;
  String get findwhat => _findwhat;
  int get flags => _flags;
  int get startIndex => _startIndex;
  bool next() => DP_Text_FindNext(_finder);
  bool prev() => DP_Text_FindPrev(_finder);
  int get resultIndex => DP_Text_GetSchResultIndex(_finder);
  int get resultCount => DP_Text_GetSchCount(_finder);
  void close() => DP_Text_FindClose(_finder);
  String toString() => "Finder(${_text},${_findwhat})";
}

// If not set, it will not match case by default.
final finderMatchCase = 0x00000001;
// If not set, it will not match the whole word by default.
final finderMatchWholeWord = 0x00000002;
// If not set, it will skip past the current match to look for the next match.
final finderConsecutive = 0x00000004;

class PageText {
  Object _text;
  Page _page;
  Object get nativeObject => _text;
  Page get page => _page;
  int get countChars => DP_Text_CountChars(_text);
  int getUnicode(int index) => DP_Text_GetUnicode(_text, index);
  double getFontSize(int index) => DP_Text_GetFontSize(_text, index);
  String getFontInfo(int index) => DP_Text_GetFontInfo(_text, index);
  double getCharAngle(int index) => DP_Text_GetCharAngle(_text, index);
  Rect getCharBox(int index) {
    List res = DP_Text_GetCharBox(_text, index);
    Rect rect = null;
    if (res != null) {
      rect = new Rect();
      rect.left = res[0];
      rect.right = res[1];
      rect.top = res[2];
      rect.bottom = res[3];
    }
    return rect;
  }

  Point getCharOrigin(int index) {
    List res = DP_Text_GetCharOrigin(_text, index);
    Point point = null;
    if (res != null) {
      point = new Point();
      point.x = res[0];
      point.y = res[1];
    }
    return point;
  }

  int getCharIndexAtPos(double x, y, xTolerance, yTolerance) => DP_Text_GetCharIndexAtPos(_text, x, y, xTolerance, yTolerance);

  String getText(int start_index, count) => DP_Text_GetText(_text, start_index, count);

  int countRects(int start_index, count) => DP_Text_CountRects(_text, start_index, count);

  Rect getRect(int rect_index) {
    List res = DP_Text_GetRect(_text, rect_index);
    Rect rect = null;
    if (res != null) {
      rect = new Rect();
      rect.left = res[0];
      rect.right = res[1];
      rect.top = res[2];
      rect.bottom = res[3];
    }
    return rect;
  }

  String getBoundedText(Rect rect) => DP_Text_GetBoundedText(_text, rect.left, rect.right, rect.top, rect.bottom);

  Finder find(String findwhat, int flags, startIndex) {
    Object f = DP_Text_FindStart(_text, findwhat, flags, startIndex);
    if (f == null) {
      return null;
    }
    Finder finder = new Finder();
    finder._text = this;
    finder._findwhat = findwhat;
    finder._flags = flags;
    finder._startIndex = startIndex;
    finder._finder = f;
    return finder;
  }

  void close() => DP_Text_ClosePage(_text);

  String toString() => "PageText(${_page._doc.filepath},${_page._index})";
}

//
// Gray scale bitmap, one byte per pixel.
const bitmapFormatGray = 1;
// 3 bytes per pixel, byte order: blue, green, red.
const bitmapFormatBGR = 2;
// 4 bytes per pixel, byte order: blue, green, red, unused.
const bitmapFormatBGRx = 3;
// 4 bytes per pixel, byte order: blue, green, red, alpha.
const bitmapFormatBGRA = 4;

class Bitmap {
  Object _bitmap;
  Bitmap(int width, height, {int alpha = 1}) {
    // Bitmap(int width, height, {int alpha = 1, int format = 0, Object firstScan = null, int stride = 0}) {
    // if (firstScan == null) {
    _bitmap = DP_Bitmap_Create(width, height, alpha);
    // } else {
    // _bitmap = DP_Bitmap_CreateEx(width, height, format, firstScan, stride);
    // }
  }
  Bitmap.fromNative(this._bitmap);
  Object get nativeObject => _bitmap;
  int get format => DP_Bitmap_GetFormat(_bitmap);
  Object get buffer => DP_Bitmap_GetBuffer(_bitmap);
  int get width => DP_Bitmap_GetWidth(_bitmap);
  int get height => DP_Bitmap_GetHeight(_bitmap);
  int get stride => DP_Bitmap_GetStride(_bitmap);
  void fillRect(int left, top, width, height, color) => DP_Bitmap_FillRect(_bitmap, left, top, width, height, color);
  void destory() => DP_Bitmap_Destroy(_bitmap);
}

class Page {
  Object _page;
  Document _doc;
  int _index;
  Object get nativeObject => _page;
  Document get document => _doc;
  int get index => _index;
  double get width => DP_GetPageWidth(_page);
  double get height => DP_GetPageHeight(_page);
  Rect get boundingBox {
    List res = DP_GetPageBoundingBox(_page);
    Rect rect = null;
    if (res != null) {
      rect = new Rect();
      rect.left = res[0];
      rect.right = res[1];
      rect.top = res[2];
      rect.bottom = res[3];
    }
    return rect;
  }

  Bitmap loadThumbnail() {
    Object b = DP_GetThumbnailAsBitmap(_page);
    if (b == null) {
      return null;
    }
    return Bitmap.fromNative(b);
  }

  PageText loadText() {
    Object t = DP_Text_LoadPage(_page);
    if (t == null) {
      return null;
    }
    PageText text = new PageText();
    text._page = this;
    text._text = t;
    return text;
  }

  void render(Bitmap bitmap, int start_x, start_y, size_x, size_y, rotate, flags) =>
      DP_RenderPageBitmap(bitmap._bitmap, _page, start_x, start_y, size_x, size_y, rotate, flags);

  void close() => DP_ClosePage(_page);

  String toString() => "Page(${_doc.filepath},$_index)";
}

class Document {
  Object _doc;
  String _filepath;
  //
  Document(String filepath, password) {
    _filepath = filepath;
    _doc = DP_LoadDocument(filepath, password);
    print(_doc);
    if (_doc == null) {
      int err = DP_LastError();
      throw new Exception("load $filepath fail wiht code $err");
    }
  }
  Object get nativeObject => _doc;

  String get filepath => _filepath;

  int get version => DP_GetFileVersion(_doc);

  int get docPermissions => DP_GetDocPermissions(_doc);

  int get securityHandlerRevision => DP_GetSecurityHandlerRevision(_doc);

  int get pageCount => DP_GetPageCount(_doc);

  Page loadPage(int index) {
    Object p = DP_LoadPage(_doc, index);
    if (p == null) {
      return null;
    }
    Page page = Page();
    page._doc = this;
    page._page = p;
    page._index = index;
    return page;
  }

  Bookmark loadBookmark() {
    Object bk = DP_Bookmark_GetFirstChild(_doc, null);
    if (bk == null) {
      return null;
    }
    Bookmark bookmark = new Bookmark();
    bookmark._doc = this;
    bookmark._bookmark = bk;
    return bookmark;
  }

  void close() => DP_CloseDocument(_doc);

  String toString() => "Document($_filepath)";
}
