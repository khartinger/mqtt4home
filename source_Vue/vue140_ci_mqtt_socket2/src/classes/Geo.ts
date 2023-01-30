// ______Geo.ts__________________________________khartinger_____
// 2023-01-02: new
// 2023-01-25: add class Geof (factor to resize), colorUnknown

// -----------font data-----------------------------------------
// examples: fh_=11, tmax_=10 or 16/13, ...
const fh_ = 11 //            font height [pixel]
const tmax_ = 10 //        max number character per line
// -----------y direction---------------------------------------
const dyl_ = Math.round(0.5 + 22 * fh_ / 14) //  line hight
const dyi_ = 5 * dyl_ //                         inner hight
const dyi2_ = Math.round(dyi_ / 2) //            half dyi_
const dyo_ = 10 * Math.round(0.5 + dyi_ / 10) // outer hight
const dyo2_ = Math.round(dyo_ / 2) //            center dy
const dym_ = Math.round((dyo_ - dyi_) / 2) //    top margin
const dyt_ = Math.round(0.5 + 17 / 14 * fh_) //  text start y
// -----------x direction---------------------------------------
const dxo_ = dyo_ //                             outer width
const dxo2_ = Math.round(dyo_ / 2) //            half outer width
const dxm_ = dym_ //                             left margin
const dxi_ = dxo_ - 2 * dxm_ //                  inner width
const dxi2_ = dxo2_ - dxm_ //                    half dxi_
const dxt_ = Math.round(0.5 + fh_ / 14 + 18 / 14) // text-border
const dw2_ = Math.round(dyl_ / 2) //             half wall thickness

// *************************************************************
// Geometric data for drawing a CI symbol (without stretching)
// *************************************************************

export class Geo {
  // =========special values====================================
  public colorOk = '#CCFFCC' //        light green 2
  public colorNotOk = '#FFCCCC' //     light red
  public colorError = '#FF3333' //     red
  public colorAlarm = '#FF3333' //     red
  public colorOn = '#FFFF66' //        yellow
  public colorOn2 = '#FFD700' //       gold
  public colorOff = '#D0D0D0' //       light grey
  public colorOff2 = '#505050' //      dark grey
  // public colorUnknown = '#add8e6' //   light blue
  public colorUnknown = '#90bbff' //   middle blue
  public colorBackground = '#DDFFDD' // very light green
  public colorOpen = '#90ee90' //      light green
  public colorClose = '#FF6666' //     light red
  public colorLock = '#C00000' //      red
  public colorWall = '#600000' //      dark red 6
  public noDate = '--.--.----'
  public noTime = '--:--:--'
  public batteryMin = 15
  // ---------texts in different languages----------------------
  public textOpen = 'AUF' // 'AUF' //    OPEN
  public textClose = 'ZU' // 'ZU' //    CLOSE
  public textLock = 'SPERRE' // 'SPERRE' // LOCK

  // =========relative geometric values=========================
  // ---------font data-----------------------------------------
  public fh = fh_ //              font height [pixel]
  public tmax = tmax_ //          max number character per line
  // ---------y direction---------------------------------------
  public dyl = dyl_ //            line hight
  public dym = dym_ //            top margin
  public dyt = dyt_ //            text start in y direction
  // ---------x direction---------------------------------------
  public dxm = dxm_ //            left margin
  public dxt = dxt_ //            text start in x direction
  // ---------other dimensions----------------------------------
  public dw2 = dw2_ //            half wall thickness

  // =========absolute geometric values=========================
  // ---------center of symbol----------------------------------
  public x = 0 //                 x value of center
  public y = 0 //                 y value of center

  // =========methods===========================================
  // _________constructor: (x/y) is center of square____________
  constructor (x_: number, y_: number) {
    this.x = x_
    this.y = y_
  }

  // ---------drawing center of symbol--------------------------
  public xc (): number { return this.x }
  public yc (lines_ = 0): number {
    if (lines_ === 1) return this.y + this.dyl / 2
    return this.y
  }

  // ---------width and height of outer rectangle---------------
  public dxo (): number { return dxo_ }
  public dyo (): number { return dyo_ }

  // ---------half width and height of outer rectangle----------
  public dxo2 (): number { return dxo2_ }
  public dyo2 (): number { return dyo2_ }

  // ---------width and height of inner rectangle---------------
  public dxi (): number { return dxi_ }
  public dyi (): number { return dyi_ }

  // ---------half width of inner rectangle---------------------
  public dxi2 (): number { return dxi2_ }
  public dyi2 (): number { return dyi2_ }

  // ---------coordinates of upper left corners-----------------
  public x0 (): number { return (this.x - this.dxo2()) }
  public y0 (): number { return (this.y - this.dyo2()) }
  public x1 (): number { return (this.x - this.dxi2()) }
  public y1 (): number { return (this.y - this.dyi2()) }

  // ---------x value for rectangle 1...(5)---------------------
  public xr (): number { return this.x1() }
  public xr2 (fx_ = 1): number { return this.x1() + fx_ * this.dxo() / 2 - this.dxm }

  // ---------calculate y start value of rectangle 1...(5)------
  public yr (linenum: number): number {
    if (linenum < 1) linenum = 1
    return (this.y1() + (linenum - 1) * this.dyl)
  }

  // ---------x start value for text line 1...(5)---------------
  public xt (): number { return (this.x + -dxi2_ + dxt_) }
  public xt2 (fx_ = 1): number { return (this.xt() + fx_ * this.dxi2()) }

  // ---------calculate y start value of text 1...(5)-----------
  public yt (linenum: number): number {
    if (linenum < 1) linenum = 1
    return (this.y - dyi2_ + dyt_ + (linenum - 1) * this.dyl)
  }

  // ---------calculate y start value of top line---------------
  // same as yt(1)
  public ytHeader (): number {
    return (this.y - dyi2_ + dyt_)
  }

  // ---------calculate y start value of center line------------
  public ytCenter (lines_ = 0): number {
    return this.yc(lines_) - this.dyl / 2 + this.dyt
  }

  // ---------calculate y start value of last (bottom) line-----
  public ytFooter (): number {
    return this.y1() + this.dyi() - this.dyl + this.dyt
  }

  // =========text methods======================================
  // ---------trim text to line length--------------------------
  public checkLen (text: string, fx_ = 1): string {
    const len = text.length
    if (len < 1) return ''
    const tmax_ = this.calctmax(fx_)
    if (len >= tmax_) return text.substring(0, tmax_)
    return text
  }

  // ---------center text (or trim text to line length)---------
  public center (text: string, fx_ = 1): string {
    const len = text.length
    if (len < 1) return ''
    const tmax_ = this.calctmax(fx_)
    if (len >= tmax_) return text.substring(0, tmax_)
    // const numBlank = Math.round((tmax_ - len - 1) / 2)
    const numBlank = Math.round((tmax_ - len - 1) / 2)
    const s1 = text.padStart(numBlank + len, ' ')
    return s1
  }

  // ---------calculate chars per line depending on fx----------
  public calctmax (fx_: number): number {
    return Math.trunc(11.9 * fx_ - 1.7)
  }

  // ---------calculate number of lines depending on fy---------
  public calcLinemax (fy_ = 1): number {
    return Math.trunc((fy_ * this.dyo() - 2 * this.dym) / this.dyl)
  }
}

// *************************************************************
// Geometric data for stretching a CI symbol
// *************************************************************

export class Geof extends Geo {
  // =========stretch factors===================================
  public fx = 1 //                     stretching factor x
  public fy = 1 //                     stretching factor y

  // =========methods===========================================
  // _________constructor: (x/y) is center of square____________
  constructor (x_ = 0, y_ = 0, fx_ = 1, fy_ = 1) {
    super(x_, y_)
    this.fx = fx_
    this.fy = fy_
  }

  // ---------x coordinate of new drawing center----------------
  public xc (): number {
    return (this.x - super.dxo2() + this.dxo2())
  }

  // ---------y coordinate of new drawing center----------------
  public yc (lines_ = 0): number {
    if (lines_ === 1) return this.y - super.dyo2() + this.dyo2() + this.dyl / 2
    return this.y - super.dyo2() + this.dyo2()
  }

  // ---------width and height of outer rectangle---------------
  public dxo (): number { return (this.fx * super.dxo()) }
  public dyo (): number { return (this.fy * super.dyo()) }

  // ---------half width and height of outer rectangle----------
  public dxo2 (): number { return (this.fx * super.dxo() / 2) }
  public dyo2 (): number { return (this.fy * super.dyo() / 2) }

  // ---------width and height of inner rectangle---------------
  public dxi (): number { return (this.fx * super.dxo() - 2 * this.dxm) }
  public dyi (): number { return (this.fy * super.dyo() - 2 * this.dym) }

  // ---------half width of inner rectangle---------------------
  public dxi2 (): number { return (this.fx * super.dxo() / 2 - this.dxm) }
  public dyi2 (): number { return (this.fy * super.dyo() / 2 - this.dym) }

  // ---------coordinates of upper left corners-----------------
  public x0 (): number { return (this.x - super.dxo2()) }
  public y0 (): number { return (this.y - super.dyo2()) }
  public x1 (): number { return (this.x - super.dxi2()) }
  public y1 (): number { return (this.y - super.dyi2()) }

  // ---------coordinates of lower right corners----------------
  public x2 (): number { return (this.x - super.dxo2() + this.dxo() - this.dxm) }
  public y2 (): number { return (this.x - super.dxo2() + this.dxo() - this.dym) }
  public x3 (): number { return (this.x - super.dxo2() + this.dxo()) }
  public y3 (): number { return (this.y - super.dyo2() + this.dyo()) }

  // ---------x value for rectangle 1...(5)---------------------
  public xr (): number { return this.x1() }
  public xr2 (): number { return this.x1() + this.dxi2() }

  // ---------calculate y start value of rectangle 1...(5)------
  public yr (linenum: number): number { return super.yr(linenum) }

  // ---------x start value for text line 1...(5)---------------
  public xt (): number { return super.xt() }
  public xt2 (): number { return (super.xt() + this.dxi2()) }

  // ---------calculate y start value of text 1...(5)-----------
  public yt (linenum: number): number { return super.yt(linenum) }

  // ---------calculate y start value of top line---------------
  public ytHeader (): number { return super.ytHeader() }

  // ---------calculate y start value of center line------------
  public ytCenter (lines_ = 0): number {
    return this.yc(lines_) - this.dyl / 2 + this.dyt
  }

  // ---------calculate y start value of last (bottom) line-----
  public ytFooter (): number {
    return this.y1() + this.dyi() - this.dyl + this.dyt
  }

  // =========text methods======================================
  // ---------trim text to line length--------------------------
  public checkLen (text: string): string { return super.checkLen(text, this.fx) }

  // ---------center text (or trim text to line length)---------
  public center (text: string): string { return super.center(text, this.fx) }

  // ---------calculate chars per line depending on fx----------
  public calctmax (): number { return super.calctmax(this.fx) }

  // ---------calculate number of lines depending on fy---------
  public calcLinemax (): number { return super.calcLinemax(this.fy) }
}

export const geo0 = new Geo(0, 0)
export const geo0f = new Geof(0, 0, 1, 1)
