######################################
# Buildroot package fragment for Pulverize
######################################

PULVERIZE_SITE_METHOD = local
# Point to the parent directory (the root of the repo)
PULVERIZE_SITE = $($(PKG)_PKGDIR)/../
PULVERIZE_DEPENDENCIES =
PULVERIZE_BUNDLES = kaoss-pulverize.lv2

# version
PULVERIZE_VERSION = 1

# toolchain PATH
PULVERIZE_TOOLCHAIN_PATH = $(HOST_DIR)/bin:$(HOST_DIR)/sbin:$(HOST_DIR)/usr/bin:$(HOST_DIR)/usr/sbin:/usr/bin:/bin

# Pass VARIANT=pulverize to the makefile
PULVERIZE_TARGET_MAKE = PATH="$(PULVERIZE_TOOLCHAIN_PATH)" \
    CC="$(TARGET_CC)" CXX="$(TARGET_CXX)" AR="$(TARGET_AR)" LD="$(TARGET_LD)" \
    PKG_CONFIG="$(TARGET_PKG_CONFIG)" \
    CFLAGS="$(TARGET_CFLAGS)" CXXFLAGS="$(TARGET_CXXFLAGS)" \
    $(MAKE) VARIANT=pulverize

define PULVERIZE_BUILD_CMDS
    $(PULVERIZE_TARGET_MAKE) -C $(@D)/source
endef

define PULVERIZE_INSTALL_TARGET_CMDS
    $(PULVERIZE_TARGET_MAKE) -C $(@D)/source install DESTDIR=$(TARGET_DIR)
endef

$(eval $(generic-package))