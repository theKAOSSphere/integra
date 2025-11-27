######################################
# Buildroot package fragment for Integra
######################################

INTEGRA_SITE_METHOD = local
INTEGRA_SITE = $($(PKG)_PKGDIR)/
INTEGRA_DEPENDENCIES =
INTEGRA_BUNDLES = integra.lv2

# version (bump to force rebuild if needed)
INTEGRA_VERSION = 1

# toolchain PATH trimmed to avoid inheriting Windows host entries with spaces
INTEGRA_TOOLCHAIN_PATH = $(HOST_DIR)/bin:$(HOST_DIR)/sbin:$(HOST_DIR)/usr/bin:$(HOST_DIR)/usr/sbin:/usr/bin:/bin

# Use standard variables for cross-compilation, explicitly passed to override environment if needed
INTEGRA_TARGET_MAKE = PATH="$(INTEGRA_TOOLCHAIN_PATH)" \
	CC="$(TARGET_CC)" CXX="$(TARGET_CXX)" AR="$(TARGET_AR)" LD="$(TARGET_LD)" \
	PKG_CONFIG="$(TARGET_PKG_CONFIG)" \
	CFLAGS="$(TARGET_CFLAGS)" CXXFLAGS="$(TARGET_CXXFLAGS)" \
	LDFLAGS="$(TARGET_LDFLAGS)" \
	$(MAKE) -C $(@D)

define INTEGRA_BUILD_CMDS
	$(INTEGRA_TARGET_MAKE)
endef

define INTEGRA_INSTALL_TARGET_CMDS
	$(INTEGRA_TARGET_MAKE) install DESTDIR=$(TARGET_DIR)
endef

$(eval $(generic-package))
