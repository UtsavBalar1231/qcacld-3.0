/*
 * Copyright (c) 2019 The Linux Foundation. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all
 * copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
/**
 * DOC: define UCFG APIs exposed by the blacklist mgr component
 */

#include <wlan_blm_ucfg_api.h>
#include <wlan_blm_core.h>

QDF_STATUS ucfg_blm_init(void)
{
	QDF_STATUS status;

	status = wlan_objmgr_register_pdev_create_handler(
			WLAN_UMAC_COMP_BLACKLIST_MGR,
			blm_pdev_object_created_notification,
			NULL);
	if (QDF_IS_STATUS_ERROR(status)) {
		blm_err("pdev create register notification failed");
		goto fail_create_pdev;
	}

	status = wlan_objmgr_register_pdev_destroy_handler(
			WLAN_UMAC_COMP_BLACKLIST_MGR,
			blm_pdev_object_destroyed_notification,
			NULL);
	if (QDF_IS_STATUS_ERROR(status)) {
		blm_err("pdev destroy register notification failed");
		goto fail_destroy_pdev;
	}

	status = wlan_objmgr_register_psoc_create_handler(
			WLAN_UMAC_COMP_BLACKLIST_MGR,
			blm_psoc_object_created_notification,
			NULL);
	if (QDF_IS_STATUS_ERROR(status)) {
		blm_err("psoc create register notification failed");
		goto fail_create_psoc;
	}

	status = wlan_objmgr_register_psoc_destroy_handler(
			WLAN_UMAC_COMP_BLACKLIST_MGR,
			blm_psoc_object_destroyed_notification,
			NULL);
	if (QDF_IS_STATUS_ERROR(status)) {
		blm_err("psoc destroy register notification failed");
		goto fail_destroy_psoc;
	}

	return QDF_STATUS_SUCCESS;

fail_destroy_psoc:
	wlan_objmgr_unregister_psoc_create_handler(WLAN_UMAC_COMP_BLACKLIST_MGR,
				   blm_psoc_object_created_notification, NULL);
fail_create_psoc:
	wlan_objmgr_unregister_pdev_destroy_handler(
				 WLAN_UMAC_COMP_BLACKLIST_MGR,
				 blm_pdev_object_destroyed_notification, NULL);
fail_destroy_pdev:
	wlan_objmgr_unregister_pdev_create_handler(WLAN_UMAC_COMP_BLACKLIST_MGR,
				   blm_pdev_object_created_notification, NULL);
fail_create_pdev:
	return status;
}

QDF_STATUS ucfg_blm_deinit(void)
{
	QDF_STATUS status;

	status = wlan_objmgr_unregister_psoc_destroy_handler(
			WLAN_UMAC_COMP_BLACKLIST_MGR,
			blm_psoc_object_destroyed_notification,
			NULL);

	status = wlan_objmgr_unregister_psoc_create_handler(
			WLAN_UMAC_COMP_BLACKLIST_MGR,
			blm_psoc_object_created_notification,
			NULL);

	status = wlan_objmgr_unregister_pdev_destroy_handler(
			WLAN_UMAC_COMP_BLACKLIST_MGR,
			blm_pdev_object_destroyed_notification,
			NULL);

	status = wlan_objmgr_unregister_pdev_create_handler(
			WLAN_UMAC_COMP_BLACKLIST_MGR,
			blm_pdev_object_created_notification,
			NULL);

	return status;
}

QDF_STATUS ucfg_blm_psoc_open(struct wlan_objmgr_psoc *psoc)
{
	return blm_cfg_psoc_open(psoc);
}

QDF_STATUS ucfg_blm_psoc_close(struct wlan_objmgr_psoc *psoc)
{
	return QDF_STATUS_SUCCESS;
}

QDF_STATUS
ucfg_blm_add_bssid_to_reject_list(struct wlan_objmgr_pdev *pdev,
				  struct reject_ap_info *ap_info)
{
	return blm_add_bssid_to_reject_list(pdev, ap_info);
}

QDF_STATUS
ucfg_blm_add_userspace_black_list(struct wlan_objmgr_pdev *pdev,
				  struct qdf_mac_addr *bssid_black_list,
				  uint8_t num_of_bssid)
{
	return blm_add_userspace_black_list(pdev, bssid_black_list,
					    num_of_bssid);
}

void
ucfg_blm_update_bssid_connect_params(struct wlan_objmgr_pdev *pdev,
				     struct qdf_mac_addr bssid,
				     enum blm_connection_state con_state)
{
	blm_update_bssid_connect_params(pdev, bssid, con_state);
}

void
ucfg_blm_flush_reject_ap_list(struct wlan_objmgr_pdev *pdev)
{
	struct blm_pdev_priv_obj *blm_ctx;

	blm_ctx = blm_get_pdev_obj(pdev);
	if (!blm_ctx) {
		blm_err("blm_ctx is NULL");
		return;
	}

	blm_flush_reject_ap_list(blm_ctx);
}