# Maintainer: David Runge <dave@sleepmap.de>

_pkgname=ebumeter
pkgname="${_pkgname}-hidpi-git"
pkgver=0.4.2
pkgrel=4
pkgdesc="Loudness measurement according to EBU-R128"
arch=('x86_64')
url="https://github.com/actionless/ebumeter-hidpi"
license=('GPL3')
groups=('pro-audio')
depends=('clxclient' 'clthreads' 'jack' 'zita-resampler')
makedepends=('gendesk')
source=(
	"${pkgname}::git+${url}.git#branch=master"
)
sha512sums=('SKIP')
conflicts=('ebumeter-git' 'ebumeter')
provides=('ebumeter')

pkgver() {
	cd "${srcdir}/${pkgname}" || exit 2
	set -o pipefail
	git describe --long | sed 's/\([^-]*-g\)/r\1/;s/-/./g' || echo 0.0.1
}

prepare() {
	cd "${srcdir}/${pkgname}" || exit 2
	#sed -e '/native/d' \
	#    -i source/Makefile
	# XDG desktop file
	gendesk -n -f \
		--pkgname ${_pkgname} \
		--pkgdesc "${pkgdesc}" \
		--name ${_pkgname} \
		--categories "AudioVideo;Audio"
}

build() {
	cd "${srcdir}/${pkgname}" || exit 2
	make -C source PREFIX=/usr
}

package() {
	cd "${srcdir}/${pkgname}" || exit 2
	make -C source PREFIX=/usr DESTDIR="$pkgdir/" install
	# XDG desktop integration
	install -vDm 644 "${_pkgname}.desktop" \
	-t "${pkgdir}/usr/share/applications/"
	install -vDm 644 {AUTHORS,doc/*.pdf} \
	-t "${pkgdir}/usr/share/doc/${pkgname}"
	install -vDm 644 CHANGELOG_original "${pkgdir}/usr/share/doc/${pkgname}/CHANGELOG"
	install -vDm 644 doc/*.{html,png} \
	-t "${pkgdir}/usr/share/doc/${pkgname}/html"
}
