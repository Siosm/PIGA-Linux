/******************************************************************************
 *
 * This file is provided under a dual BSD/GPLv2 license.  When using or
 * redistributing this file, you may do so under either license.
 *
 * GPL LICENSE SUMMARY
 *
 * Copyright(c) 2008 - 2010 Intel Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110,
 * USA
 *
 * The full GNU General Public License is included in this distribution
 * in the file called LICENSE.GPL.
 *
 * Contact Information:
 *  Intel Linux Wireless <ilw@linux.intel.com>
 * Intel Corporation, 5200 N.E. Elam Young Parkway, Hillsboro, OR 97124-6497
 *
 * BSD LICENSE
 *
 * Copyright(c) 2005 - 2010 Intel Corporation. All rights reserved.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name Intel Corporation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#ifndef __iwl_core_h__
#define __iwl_core_h__

#include "iwl-dev.h"

/************************
 * forward declarations *
 ************************/
struct iwl_host_cmd;
struct iwl_cmd;


#define IWLWIFI_VERSION "in-tree:"
#define DRV_COPYRIGHT	"Copyright(c) 2003-2010 Intel Corporation"
#define DRV_AUTHOR     "<ilw@linux.intel.com>"

#define IWL_PCI_DEVICE(dev, subdev, cfg) \
	.vendor = PCI_VENDOR_ID_INTEL,  .device = (dev), \
	.subvendor = PCI_ANY_ID, .subdevice = (subdev), \
	.driver_data = (kernel_ulong_t)&(cfg)

#define TIME_UNIT		1024

#define IWL_SKU_G       0x1
#define IWL_SKU_A       0x2
#define IWL_SKU_N       0x8

#define IWL_CMD(x) case x: return #x

struct iwl_hcmd_ops {
	int (*rxon_assoc)(struct iwl_priv *priv, struct iwl_rxon_context *ctx);
	int (*commit_rxon)(struct iwl_priv *priv, struct iwl_rxon_context *ctx);
	void (*set_rxon_chain)(struct iwl_priv *priv,
			       struct iwl_rxon_context *ctx);
	int (*set_tx_ant)(struct iwl_priv *priv, u8 valid_tx_ant);
	void (*send_bt_config)(struct iwl_priv *priv);
	int (*set_pan_params)(struct iwl_priv *priv);
};

struct iwl_hcmd_utils_ops {
	u16 (*get_hcmd_size)(u8 cmd_id, u16 len);
	u16 (*build_addsta_hcmd)(const struct iwl_addsta_cmd *cmd, u8 *data);
	void (*gain_computation)(struct iwl_priv *priv,
			u32 *average_noise,
			u16 min_average_noise_antennat_i,
			u32 min_average_noise,
			u8 default_chain);
	void (*chain_noise_reset)(struct iwl_priv *priv);
	void (*tx_cmd_protection)(struct iwl_priv *priv,
				  struct ieee80211_tx_info *info,
				  __le16 fc, __le32 *tx_flags);
	int  (*calc_rssi)(struct iwl_priv *priv,
			  struct iwl_rx_phy_res *rx_resp);
	int (*request_scan)(struct iwl_priv *priv, struct ieee80211_vif *vif);
	void (*post_scan)(struct iwl_priv *priv);
};

struct iwl_apm_ops {
	int (*init)(struct iwl_priv *priv);
	void (*config)(struct iwl_priv *priv);
};

struct iwl_isr_ops {
	irqreturn_t (*isr) (int irq, void *data);
	void (*free)(struct iwl_priv *priv);
	int (*alloc)(struct iwl_priv *priv);
	int (*reset)(struct iwl_priv *priv);
	void (*disable)(struct iwl_priv *priv);
};

struct iwl_debugfs_ops {
	ssize_t (*rx_stats_read)(struct file *file, char __user *user_buf,
				 size_t count, loff_t *ppos);
	ssize_t (*tx_stats_read)(struct file *file, char __user *user_buf,
				 size_t count, loff_t *ppos);
	ssize_t (*general_stats_read)(struct file *file, char __user *user_buf,
				      size_t count, loff_t *ppos);
	ssize_t (*bt_stats_read)(struct file *file, char __user *user_buf,
				 size_t count, loff_t *ppos);
	ssize_t (*reply_tx_error)(struct file *file, char __user *user_buf,
				 size_t count, loff_t *ppos);
};

struct iwl_temp_ops {
	void (*temperature)(struct iwl_priv *priv);
};

struct iwl_tt_ops {
	bool (*lower_power_detection)(struct iwl_priv *priv);
	u8 (*tt_power_mode)(struct iwl_priv *priv);
	bool (*ct_kill_check)(struct iwl_priv *priv);
};

struct iwl_lib_ops {
	/* set hw dependent parameters */
	int (*set_hw_params)(struct iwl_priv *priv);
	/* Handling TX */
	void (*txq_update_byte_cnt_tbl)(struct iwl_priv *priv,
					struct iwl_tx_queue *txq,
					u16 byte_cnt);
	void (*txq_inval_byte_cnt_tbl)(struct iwl_priv *priv,
				       struct iwl_tx_queue *txq);
	void (*txq_set_sched)(struct iwl_priv *priv, u32 mask);
	int (*txq_attach_buf_to_tfd)(struct iwl_priv *priv,
				     struct iwl_tx_queue *txq,
				     dma_addr_t addr,
				     u16 len, u8 reset, u8 pad);
	void (*txq_free_tfd)(struct iwl_priv *priv,
			     struct iwl_tx_queue *txq);
	int (*txq_init)(struct iwl_priv *priv,
			struct iwl_tx_queue *txq);
	/* aggregations */
	int (*txq_agg_enable)(struct iwl_priv *priv, int txq_id, int tx_fifo,
			      int sta_id, int tid, u16 ssn_idx);
	int (*txq_agg_disable)(struct iwl_priv *priv, u16 txq_id, u16 ssn_idx,
			       u8 tx_fifo);
	/* setup Rx handler */
	void (*rx_handler_setup)(struct iwl_priv *priv);
	/* setup deferred work */
	void (*setup_deferred_work)(struct iwl_priv *priv);
	/* cancel deferred work */
	void (*cancel_deferred_work)(struct iwl_priv *priv);
	/* alive notification after init uCode load */
	void (*init_alive_start)(struct iwl_priv *priv);
	/* alive notification */
	int (*alive_notify)(struct iwl_priv *priv);
	/* check validity of rtc data address */
	int (*is_valid_rtc_data_addr)(u32 addr);
	/* 1st ucode load */
	int (*load_ucode)(struct iwl_priv *priv);
	int (*dump_nic_event_log)(struct iwl_priv *priv,
				  bool full_log, char **buf, bool display);
	void (*dump_nic_error_log)(struct iwl_priv *priv);
	void (*dump_csr)(struct iwl_priv *priv);
	int (*dump_fh)(struct iwl_priv *priv, char **buf, bool display);
	int (*set_channel_switch)(struct iwl_priv *priv,
				  struct ieee80211_channel_switch *ch_switch);
	/* power management */
	const struct iwl_apm_ops apm_ops;

	/* power */
	int (*send_tx_power) (struct iwl_priv *priv);
	void (*update_chain_flags)(struct iwl_priv *priv);

	/* isr */
	const struct iwl_isr_ops isr_ops;

	/* eeprom operations (as defined in iwl-eeprom.h) */
	struct iwl_eeprom_ops eeprom_ops;

	/* temperature */
	const struct iwl_temp_ops temp_ops;

	int (*txfifo_flush)(struct iwl_priv *priv, u16 flush_control);
	void (*dev_txfifo_flush)(struct iwl_priv *priv, u16 flush_control);

	const struct iwl_debugfs_ops debugfs_ops;const struct iwl_tt_ops tt_ops;
};

struct iwl_led_ops {
	int (*cmd)(struct iwl_priv *priv, struct iwl_led_cmd *led_cmd);
};

/* NIC specific ops */
struct iwl_nic_ops {
	void (*additional_nic_config)(struct iwl_priv *priv);
};

struct iwl_legacy_ops {
	void (*post_associate)(struct iwl_priv *priv);
	void (*config_ap)(struct iwl_priv *priv);
	/* station management */
	int (*update_bcast_stations)(struct iwl_priv *priv);
	int (*manage_ibss_station)(struct iwl_priv *priv,
				   struct ieee80211_vif *vif, bool add);
};

struct iwl_ops {
	const struct iwl_lib_ops *lib;
	const struct iwl_hcmd_ops *hcmd;
	const struct iwl_hcmd_utils_ops *utils;
	const struct iwl_led_ops *led;
	const struct iwl_nic_ops *nic;
	const struct iwl_legacy_ops *legacy;
	const struct ieee80211_ops *ieee80211_ops;
};

struct iwl_mod_params {
	int sw_crypto;		/* def: 0 = using hardware encryption */
	int disable_hw_scan;	/* def: 0 = use h/w scan */
	int num_of_queues;	/* def: HW dependent */
	int disable_11n;	/* def: 0 = 11n capabilities enabled */
	int amsdu_size_8K;	/* def: 1 = enable 8K amsdu size */
	int antenna;  		/* def: 0 = both antennas (use diversity) */
	int restart_fw;		/* def: 1 = restart firmware */
	bool plcp_check;	/* def: true = enable plcp health check */
	bool ack_check;		/* def: false = disable ack health check */
};

/*
 * @max_ll_items: max number of OTP blocks
 * @shadow_ram_support: shadow support for OTP memory
 * @led_compensation: compensate on the led on/off time per HW according
 *	to the deviation to achieve the desired led frequency.
 *	The detail algorithm is described in iwl-led.c
 * @chain_noise_num_beacons: number of beacons used to compute chain noise
 * @adv_thermal_throttle: support advance thermal throttle
 * @support_ct_kill_exit: support ct kill exit condition
 * @support_wimax_coexist: support wimax/wifi co-exist
 * @plcp_delta_threshold: plcp error rate threshold used to trigger
 *	radio tuning when there is a high receiving plcp error rate
 * @chain_noise_scale: default chain noise scale used for gain computation
 * @wd_timeout: TX queues watchdog timeout
 * @temperature_kelvin: temperature report by uCode in kelvin
 * @max_event_log_size: size of event log buffer size for ucode event logging
 * @tx_power_by_driver: tx power calibration performed by driver
 *	instead of uCode
 * @ucode_tracing: support ucode continuous tracing
 * @sensitivity_calib_by_driver: driver has the capability to perform
 *	sensitivity calibration operation
 * @chain_noise_calib_by_driver: driver has the capability to perform
 *	chain noise calibration operation
 * @shadow_reg_enable: HW shadhow register bit
 * @no_agg_framecnt_info: uCode do not provide aggregation frame count
 *	information
 */
struct iwl_base_params {
	int eeprom_size;
	int num_of_queues;	/* def: HW dependent */
	int num_of_ampdu_queues;/* def: HW dependent */
	/* for iwl_apm_init() */
	u32 pll_cfg_val;
	bool set_l0s;
	bool use_bsm;

	bool use_isr_legacy;
	const u16 max_ll_items;
	const bool shadow_ram_support;
	u16 led_compensation;
	const bool broken_powersave;
	int chain_noise_num_beacons;
	bool adv_thermal_throttle;
	bool support_ct_kill_exit;
	const bool support_wimax_coexist;
	u8 plcp_delta_threshold;
	s32 chain_noise_scale;
	unsigned int wd_timeout;
	bool temperature_kelvin;
	u32 max_event_log_size;
	const bool tx_power_by_driver;
	const bool ucode_tracing;
	const bool sensitivity_calib_by_driver;
	const bool chain_noise_calib_by_driver;
	const bool shadow_reg_enable;
	const bool no_agg_framecnt_info;
};
/*
 * @advanced_bt_coexist: support advanced bt coexist
 * @bt_init_traffic_load: specify initial bt traffic load
 * @bt_prio_boost: default bt priority boost value
 * @bt_statistics: use BT version of statistics notification
 * @agg_time_limit: maximum number of uSec in aggregation
 * @ampdu_factor: Maximum A-MPDU length factor
 * @ampdu_density: Minimum A-MPDU spacing
 * @bt_sco_disable: uCode should not response to BT in SCO/ESCO mode
*/
struct iwl_bt_params {
	bool advanced_bt_coexist;
	u8 bt_init_traffic_load;
	u8 bt_prio_boost;
	const bool bt_statistics;
	u16 agg_time_limit;
	u8 ampdu_factor;
	u8 ampdu_density;
	bool bt_sco_disable;
	bool bt_session_2;
};
/*
 * @use_rts_for_aggregation: use rts/cts protection for HT traffic
*/
struct iwl_ht_params {
	const bool ht_greenfield_support; /* if used set to true */
	bool use_rts_for_aggregation;
};

/**
 * struct iwl_cfg
 * @fw_name_pre: Firmware filename prefix. The api version and extension
 *	(.ucode) will be added to filename before loading from disk. The
 *	filename is constructed as fw_name_pre<api>.ucode.
 * @ucode_api_max: Highest version of uCode API supported by driver.
 * @ucode_api_min: Lowest version of uCode API supported by driver.
 * @pa_type: used by 6000 series only to identify the type of Power Amplifier
 * @need_dc_calib: need to perform init dc calibration
 * @need_temp_offset_calib: need to perform temperature offset calibration
 * @scan_antennas: available antenna for scan operation
 * @led_mode: 0=blinking, 1=On(RF On)/Off(RF Off)
 * @adv_pm: advance power management
 * @rx_with_siso_diversity: 1x1 device with rx antenna diversity
 * @internal_wimax_coex: internal wifi/wimax combo device
 * @iq_invert: I/Q inversion
 *
 * We enable the driver to be backward compatible wrt API version. The
 * driver specifies which APIs it supports (with @ucode_api_max being the
 * highest and @ucode_api_min the lowest). Firmware will only be loaded if
 * it has a supported API version. The firmware's API version will be
 * stored in @iwl_priv, enabling the driver to make runtime changes based
 * on firmware version used.
 *
 * For example,
 * if (IWL_UCODE_API(priv->ucode_ver) >= 2) {
 *	Driver interacts with Firmware API version >= 2.
 * } else {
 *	Driver interacts with Firmware API version 1.
 * }
 *
 * The ideal usage of this infrastructure is to treat a new ucode API
 * release as a new hardware revision. That is, through utilizing the
 * iwl_hcmd_utils_ops etc. we accommodate different command structures
 * and flows between hardware versions (4965/5000) as well as their API
 * versions.
 *
 */
struct iwl_cfg {
	/* params specific to an individual device within a device family */
	const char *name;
	const char *fw_name_pre;
	const unsigned int ucode_api_max;
	const unsigned int ucode_api_min;
	u8   valid_tx_ant;
	u8   valid_rx_ant;
	unsigned int sku;
	u16  eeprom_ver;
	u16  eeprom_calib_ver;
	const struct iwl_ops *ops;
	/* module based parameters which can be set from modprobe cmd */
	const struct iwl_mod_params *mod_params;
	/* params not likely to change within a device family */
	struct iwl_base_params *base_params;
	/* params likely to change within a device family */
	struct iwl_ht_params *ht_params;
	struct iwl_bt_params *bt_params;
	enum iwl_pa_type pa_type;	  /* if used set to IWL_PA_SYSTEM */
	const bool need_dc_calib;	  /* if used set to true */
	const bool need_temp_offset_calib; /* if used set to true */
	u8 scan_rx_antennas[IEEE80211_NUM_BANDS];
	enum iwl_led_mode led_mode;
	const bool adv_pm;
	const bool rx_with_siso_diversity;
	const bool internal_wimax_coex;
	const bool iq_invert;
};

/***************************
 *   L i b                 *
 ***************************/

struct ieee80211_hw *iwl_alloc_all(struct iwl_cfg *cfg);
int iwl_mac_conf_tx(struct ieee80211_hw *hw, u16 queue,
		    const struct ieee80211_tx_queue_params *params);
int iwl_mac_tx_last_beacon(struct ieee80211_hw *hw);
void iwl_set_rxon_hwcrypto(struct iwl_priv *priv, struct iwl_rxon_context *ctx,
			   int hw_decrypt);
int iwl_check_rxon_cmd(struct iwl_priv *priv, struct iwl_rxon_context *ctx);
int iwl_full_rxon_required(struct iwl_priv *priv, struct iwl_rxon_context *ctx);
int iwl_set_rxon_channel(struct iwl_priv *priv, struct ieee80211_channel *ch,
			 struct iwl_rxon_context *ctx);
void iwl_set_flags_for_band(struct iwl_priv *priv,
			    struct iwl_rxon_context *ctx,
			    enum ieee80211_band band,
			    struct ieee80211_vif *vif);
u8 iwl_get_single_channel_number(struct iwl_priv *priv,
				  enum ieee80211_band band);
void iwl_set_rxon_ht(struct iwl_priv *priv, struct iwl_ht_config *ht_conf);
bool iwl_is_ht40_tx_allowed(struct iwl_priv *priv,
			    struct iwl_rxon_context *ctx,
			    struct ieee80211_sta_ht_cap *ht_cap);
void iwl_connection_init_rx_config(struct iwl_priv *priv,
				   struct iwl_rxon_context *ctx);
void iwl_set_rate(struct iwl_priv *priv);
void iwl_irq_handle_error(struct iwl_priv *priv);
int iwl_mac_add_interface(struct ieee80211_hw *hw,
			  struct ieee80211_vif *vif);
void iwl_mac_remove_interface(struct ieee80211_hw *hw,
			      struct ieee80211_vif *vif);
int iwl_mac_change_interface(struct ieee80211_hw *hw,
			     struct ieee80211_vif *vif,
			     enum nl80211_iftype newtype, bool newp2p);
int iwl_alloc_txq_mem(struct iwl_priv *priv);
void iwl_free_txq_mem(struct iwl_priv *priv);

#ifdef CONFIG_IWLWIFI_DEBUGFS
int iwl_alloc_traffic_mem(struct iwl_priv *priv);
void iwl_free_traffic_mem(struct iwl_priv *priv);
void iwl_reset_traffic_log(struct iwl_priv *priv);
void iwl_dbg_log_tx_data_frame(struct iwl_priv *priv,
				u16 length, struct ieee80211_hdr *header);
void iwl_dbg_log_rx_data_frame(struct iwl_priv *priv,
				u16 length, struct ieee80211_hdr *header);
const char *get_mgmt_string(int cmd);
const char *get_ctrl_string(int cmd);
void iwl_clear_traffic_stats(struct iwl_priv *priv);
void iwl_update_stats(struct iwl_priv *priv, bool is_tx, __le16 fc,
		      u16 len);
#else
static inline int iwl_alloc_traffic_mem(struct iwl_priv *priv)
{
	return 0;
}
static inline void iwl_free_traffic_mem(struct iwl_priv *priv)
{
}
static inline void iwl_reset_traffic_log(struct iwl_priv *priv)
{
}
static inline void iwl_dbg_log_tx_data_frame(struct iwl_priv *priv,
		      u16 length, struct ieee80211_hdr *header)
{
}
static inline void iwl_dbg_log_rx_data_frame(struct iwl_priv *priv,
		      u16 length, struct ieee80211_hdr *header)
{
}
static inline void iwl_update_stats(struct iwl_priv *priv, bool is_tx,
				    __le16 fc, u16 len)
{
}
#endif

/*****************************************************
* RX
******************************************************/
void iwl_cmd_queue_free(struct iwl_priv *priv);
void iwl_cmd_queue_unmap(struct iwl_priv *priv);
int iwl_rx_queue_alloc(struct iwl_priv *priv);
void iwl_rx_queue_update_write_ptr(struct iwl_priv *priv,
				  struct iwl_rx_queue *q);
int iwl_rx_queue_space(const struct iwl_rx_queue *q);
void iwl_tx_cmd_complete(struct iwl_priv *priv, struct iwl_rx_mem_buffer *rxb);

void iwl_chswitch_done(struct iwl_priv *priv, bool is_success);

/* TX helpers */

/*****************************************************
* TX
******************************************************/
void iwl_txq_update_write_ptr(struct iwl_priv *priv, struct iwl_tx_queue *txq);
int iwl_tx_queue_init(struct iwl_priv *priv, struct iwl_tx_queue *txq,
		      int slots_num, u32 txq_id);
void iwl_tx_queue_reset(struct iwl_priv *priv, struct iwl_tx_queue *txq,
			int slots_num, u32 txq_id);
void iwl_tx_queue_free(struct iwl_priv *priv, int txq_id);
void iwl_tx_queue_unmap(struct iwl_priv *priv, int txq_id);
void iwl_setup_watchdog(struct iwl_priv *priv);
/*****************************************************
 * TX power
 ****************************************************/
int iwl_set_tx_power(struct iwl_priv *priv, s8 tx_power, bool force);

/*******************************************************************************
 * Rate
 ******************************************************************************/

u8 iwl_rate_get_lowest_plcp(struct iwl_priv *priv,
			    struct iwl_rxon_context *ctx);

/*******************************************************************************
 * Scanning
 ******************************************************************************/
void iwl_init_scan_params(struct iwl_priv *priv);
int iwl_scan_cancel(struct iwl_priv *priv);
int iwl_scan_cancel_timeout(struct iwl_priv *priv, unsigned long ms);
void iwl_force_scan_end(struct iwl_priv *priv);
int iwl_mac_hw_scan(struct ieee80211_hw *hw,
		    struct ieee80211_vif *vif,
		    struct cfg80211_scan_request *req);
void iwl_internal_short_hw_scan(struct iwl_priv *priv);
int iwl_force_reset(struct iwl_priv *priv, int mode, bool external);
u16 iwl_fill_probe_req(struct iwl_priv *priv, struct ieee80211_mgmt *frame,
		       const u8 *ta, const u8 *ie, int ie_len, int left);
void iwl_setup_rx_scan_handlers(struct iwl_priv *priv);
u16 iwl_get_active_dwell_time(struct iwl_priv *priv,
			      enum ieee80211_band band,
			      u8 n_probes);
u16 iwl_get_passive_dwell_time(struct iwl_priv *priv,
			       enum ieee80211_band band,
			       struct ieee80211_vif *vif);
void iwl_setup_scan_deferred_work(struct iwl_priv *priv);
void iwl_cancel_scan_deferred_work(struct iwl_priv *priv);
int __must_check iwl_scan_initiate(struct iwl_priv *priv,
				   struct ieee80211_vif *vif,
				   enum iwl_scan_type scan_type,
				   enum ieee80211_band band);

/* For faster active scanning, scan will move to the next channel if fewer than
 * PLCP_QUIET_THRESH packets are heard on this channel within
 * ACTIVE_QUIET_TIME after sending probe request.  This shortens the dwell
 * time if it's a quiet channel (nothing responded to our probe, and there's
 * no other traffic).
 * Disable "quiet" feature by setting PLCP_QUIET_THRESH to 0. */
#define IWL_ACTIVE_QUIET_TIME       cpu_to_le16(10)  /* msec */
#define IWL_PLCP_QUIET_THRESH       cpu_to_le16(1)  /* packets */

#define IWL_SCAN_CHECK_WATCHDOG		(HZ * 7)

/*****************************************************
 *   S e n d i n g     H o s t     C o m m a n d s   *
 *****************************************************/

const char *get_cmd_string(u8 cmd);
int __must_check iwl_send_cmd_sync(struct iwl_priv *priv,
				   struct iwl_host_cmd *cmd);
int iwl_send_cmd(struct iwl_priv *priv, struct iwl_host_cmd *cmd);
int __must_check iwl_send_cmd_pdu(struct iwl_priv *priv, u8 id,
				  u16 len, const void *data);
int iwl_send_cmd_pdu_async(struct iwl_priv *priv, u8 id, u16 len,
			   const void *data,
			   void (*callback)(struct iwl_priv *priv,
					    struct iwl_device_cmd *cmd,
					    struct iwl_rx_packet *pkt));

int iwl_enqueue_hcmd(struct iwl_priv *priv, struct iwl_host_cmd *cmd);


/*****************************************************
 * PCI						     *
 *****************************************************/

static inline u16 iwl_pcie_link_ctl(struct iwl_priv *priv)
{
	int pos;
	u16 pci_lnk_ctl;
	pos = pci_find_capability(priv->pci_dev, PCI_CAP_ID_EXP);
	pci_read_config_word(priv->pci_dev, pos + PCI_EXP_LNKCTL, &pci_lnk_ctl);
	return pci_lnk_ctl;
}

void iwl_bg_watchdog(unsigned long data);
u32 iwl_usecs_to_beacons(struct iwl_priv *priv, u32 usec, u32 beacon_interval);
__le32 iwl_add_beacon_time(struct iwl_priv *priv, u32 base,
			   u32 addon, u32 beacon_interval);

#ifdef CONFIG_PM
int iwl_pci_suspend(struct device *device);
int iwl_pci_resume(struct device *device);
extern const struct dev_pm_ops iwl_pm_ops;

#define IWL_PM_OPS	(&iwl_pm_ops)

#else /* !CONFIG_PM */

#define IWL_PM_OPS	NULL

#endif /* !CONFIG_PM */

/*****************************************************
*  Error Handling Debugging
******************************************************/
void iwl_dump_nic_error_log(struct iwl_priv *priv);
int iwl_dump_nic_event_log(struct iwl_priv *priv,
			   bool full_log, char **buf, bool display);
#ifdef CONFIG_IWLWIFI_DEBUG
void iwl_print_rx_config_cmd(struct iwl_priv *priv,
			     struct iwl_rxon_context *ctx);
#else
static inline void iwl_print_rx_config_cmd(struct iwl_priv *priv,
					   struct iwl_rxon_context *ctx)
{
}
#endif

void iwl_clear_isr_stats(struct iwl_priv *priv);

/*****************************************************
*  GEOS
******************************************************/
int iwlcore_init_geos(struct iwl_priv *priv);
void iwlcore_free_geos(struct iwl_priv *priv);

/*************** DRIVER STATUS FUNCTIONS   *****/

#define STATUS_HCMD_ACTIVE	0	/* host command in progress */
/* 1 is unused (used to be STATUS_HCMD_SYNC_ACTIVE) */
#define STATUS_INT_ENABLED	2
#define STATUS_RF_KILL_HW	3
#define STATUS_CT_KILL		4
#define STATUS_INIT		5
#define STATUS_ALIVE		6
#define STATUS_READY		7
#define STATUS_TEMPERATURE	8
#define STATUS_GEO_CONFIGURED	9
#define STATUS_EXIT_PENDING	10
#define STATUS_STATISTICS	12
#define STATUS_SCANNING		13
#define STATUS_SCAN_ABORTING	14
#define STATUS_SCAN_HW		15
#define STATUS_POWER_PMI	16
#define STATUS_FW_ERROR		17
#define STATUS_CHANNEL_SWITCH_PENDING 19

static inline int iwl_is_ready(struct iwl_priv *priv)
{
	/* The adapter is 'ready' if READY and GEO_CONFIGURED bits are
	 * set but EXIT_PENDING is not */
	return test_bit(STATUS_READY, &priv->status) &&
	       test_bit(STATUS_GEO_CONFIGURED, &priv->status) &&
	       !test_bit(STATUS_EXIT_PENDING, &priv->status);
}

static inline int iwl_is_alive(struct iwl_priv *priv)
{
	return test_bit(STATUS_ALIVE, &priv->status);
}

static inline int iwl_is_init(struct iwl_priv *priv)
{
	return test_bit(STATUS_INIT, &priv->status);
}

static inline int iwl_is_rfkill_hw(struct iwl_priv *priv)
{
	return test_bit(STATUS_RF_KILL_HW, &priv->status);
}

static inline int iwl_is_rfkill(struct iwl_priv *priv)
{
	return iwl_is_rfkill_hw(priv);
}

static inline int iwl_is_ctkill(struct iwl_priv *priv)
{
	return test_bit(STATUS_CT_KILL, &priv->status);
}

static inline int iwl_is_ready_rf(struct iwl_priv *priv)
{

	if (iwl_is_rfkill(priv))
		return 0;

	return iwl_is_ready(priv);
}

extern void iwl_send_bt_config(struct iwl_priv *priv);
extern int iwl_send_statistics_request(struct iwl_priv *priv,
				       u8 flags, bool clear);
void iwl_apm_stop(struct iwl_priv *priv);
int iwl_apm_init(struct iwl_priv *priv);

int iwl_send_rxon_timing(struct iwl_priv *priv, struct iwl_rxon_context *ctx);
static inline int iwl_send_rxon_assoc(struct iwl_priv *priv,
				      struct iwl_rxon_context *ctx)
{
	return priv->cfg->ops->hcmd->rxon_assoc(priv, ctx);
}
static inline int iwlcore_commit_rxon(struct iwl_priv *priv,
				      struct iwl_rxon_context *ctx)
{
	return priv->cfg->ops->hcmd->commit_rxon(priv, ctx);
}
static inline const struct ieee80211_supported_band *iwl_get_hw_mode(
			struct iwl_priv *priv, enum ieee80211_band band)
{
	return priv->hw->wiphy->bands[band];
}

static inline bool iwl_advanced_bt_coexist(struct iwl_priv *priv)
{
	return priv->cfg->bt_params &&
	       priv->cfg->bt_params->advanced_bt_coexist;
}

static inline bool iwl_bt_statistics(struct iwl_priv *priv)
{
	return priv->cfg->bt_params && priv->cfg->bt_params->bt_statistics;
}

extern bool bt_coex_active;
extern bool bt_siso_mode;

#endif /* __iwl_core_h__ */
