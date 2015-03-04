/*
 * Copyright (c) 2006, 2008, Oracle and/or its affiliates. All rights reserved.
 */

#include "drmP.h"
#include <sys/kstat.h>
#include <sys/ddi.h>
#include <sys/sunddi.h>
#include <sys/sunldi.h>

static char *drmkstat_name[] = {
	"opens",
	"closes",
	"IOCTLs",
	"locks",
	"unlocks",
	NULL
};

static int
drm_kstat_update(kstat_t *ksp, int flag)
{
	drm_device_t *sc;
	kstat_named_t *knp;
	int tmp;

	if (flag != KSTAT_READ)
		return (EACCES);

	sc = ksp->ks_private;
	knp = ksp->ks_data;

	for (tmp = 1; tmp < 6; tmp++) {
		(knp++)->value.ui32 = sc->counts[tmp];
	}

	return (0);
}

int
drm_init_kstats(drm_device_t *sc)
{
	int instance;
	kstat_t *ksp;
	kstat_named_t *knp;
	char *np;
	char **aknp;

	instance = ddi_get_instance(sc->dip);
	aknp = drmkstat_name;
	ksp = kstat_create("drm", instance, "drminfo", "drm",
	    KSTAT_TYPE_NAMED, sizeof (drmkstat_name)/sizeof (char *) - 1,
	    KSTAT_FLAG_PERSISTENT);
	if (ksp == NULL)
		return (NULL);

	ksp->ks_private = sc;
	ksp->ks_update = drm_kstat_update;
	for (knp = ksp->ks_data; (np = (*aknp)) != NULL; knp++, aknp++) {
		kstat_named_init(knp, np, KSTAT_DATA_UINT32);
	}
	kstat_install(ksp);

	sc->asoft_ksp = ksp;

	return (0);
}

void
drm_fini_kstats(drm_device_t *sc)
{
	if (sc->asoft_ksp)
		kstat_delete(sc->asoft_ksp);
	else
		cmn_err(CE_WARN, "attempt to delete null kstat");
}
