part of dart.pdfium;

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
  Bitmap(int width, height, bool alpha) {
    // Bitmap(int width, height, {int alpha = 1, int format = 0, Object firstScan = null, int stride = 0}) {
    // if (firstScan == null) {
    _bitmap = DP_Bitmap_Create(width, height, alpha ? 1 : 0);
    // } else {
    // _bitmap = DP_Bitmap_CreateEx(width, height, format, firstScan, stride);
    // }
  }
  Bitmap.fromNative(this._bitmap);
  Object get nativeObject => _bitmap;
  int get format => DP_Bitmap_GetFormat(_bitmap);
  Uint8List get buffer => DP_Bitmap_GetBuffer(_bitmap);
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

  bool get transparency => DP_Page_HasTransparency(_page);

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
