/*
 * COPYRIGHT 2015 SEAGATE LLC
 *
 * THIS DRAWING/DOCUMENT, ITS SPECIFICATIONS, AND THE DATA CONTAINED
 * HEREIN, ARE THE EXCLUSIVE PROPERTY OF SEAGATE TECHNOLOGY
 * LIMITED, ISSUED IN STRICT CONFIDENCE AND SHALL NOT, WITHOUT
 * THE PRIOR WRITTEN PERMISSION OF SEAGATE TECHNOLOGY LIMITED,
 * BE REPRODUCED, COPIED, OR DISCLOSED TO A THIRD PARTY, OR
 * USED FOR ANY PURPOSE WHATSOEVER, OR STORED IN A RETRIEVAL SYSTEM
 * EXCEPT AS ALLOWED BY THE TERMS OF SEAGATE LICENSES AND AGREEMENTS.
 *
 * YOU SHOULD HAVE RECEIVED A COPY OF SEAGATE'S LICENSE ALONG WITH
 * THIS RELEASE. IF NOT PLEASE CONTACT A SEAGATE REPRESENTATIVE
 * http://www.seagate.com/contact
 *
 * Original author:  Rajesh Nambiar   <rajesh.nambiar@seagate.com>
 * Original creation date: 16-March-2016
 */
#pragma once

#ifndef __S3_SERVER_S3_OPTION_H__
#define __S3_SERVER_S3_OPTION_H__

#define S3_OPTION_BIND_ADDR 0x0001
#define S3_OPTION_BIND_PORT 0x0002
#define S3_OPTION_CLOVIS_LOCAL_ADDR 0x0004
#define S3_OPTION_PLACE_HOLDER_1 0x0008
#define S3_OPTION_CLOVIS_HA_ADDR 0x0010
#define S3_OPTION_AUTH_IP_ADDR 0x0020
#define S3_OPTION_AUTH_PORT 0x0040
#define S3_CLOVIS_LAYOUT_ID 0x0080
#define S3_OPTION_LOG_DIR 0x0100
#define S3_OPTION_LOG_MODE 0x0200
#define S3_OPTION_PERF_LOG_FILE 0x0400
#define S3_OPTION_LOG_FILE_MAX_SIZE 0x0800
#define S3_OPTION_PIDFILE 0x01000
#define S3_OPTION_STATSD_IP_ADDR 0x2000
#define S3_OPTION_STATSD_PORT 0x4000
#define S3_OPTION_CLOVIS_PROF 0x8000
#define S3_OPTION_CLOVIS_PROCESS_FID 0x10000
#define S3_OPTION_REUSEPORT 0x20000

#define S3_OPTION_ASSERT_AND_RET(node, option)                              \
  do {                                                                      \
    if (!node[option]) {                                                    \
      fprintf(stderr, "%s:%d:option [%s] is missing\n", __FILE__, __LINE__, \
              option);                                                      \
      return false;                                                         \
    }                                                                       \
  } while (0)

#include <gtest/gtest_prod.h>
#include <limits.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <set>
#include <string>
#include "evhtp_wrapper.h"
#include "s3_cli_options.h"

class S3Option {
  int cmd_opt_flag;

  std::string s3_nodename;
  std::string s3_bind_addr;
  std::string s3_pidfile;
  unsigned short s3_bind_port;
  unsigned short max_retry_count;
  unsigned short retry_interval_millisec;

  std::string auth_ip_addr;
  unsigned short auth_port;

  std::string s3_default_endpoint;
  std::set<std::string> s3_region_endpoints;
  unsigned short s3_grace_period_sec;
  bool is_s3_shutting_down;

  unsigned short perf_enabled;
  std::string perf_log_file;

  std::string option_file;
  std::string log_dir;
  std::string layout_recommendation_file;
  std::string s3_iam_cert_file;

  int read_ahead_multiple;
  std::string log_level;
  int log_file_max_size_mb;
  bool s3_enable_auth_ssl;
  bool s3_reuseport;
  bool log_buffering_enable;
  bool s3_enable_murmurhash_oid;
  int log_flush_frequency_sec;

  unsigned short clovis_layout_id;
  unsigned short clovis_units_per_request;
  std::vector<int> clovis_unit_sizes_for_mem_pool;
  int clovis_idx_fetch_count;
  std::string clovis_local_addr;
  std::string clovis_ha_addr;
  std::string clovis_profile;
  bool clovis_is_oostore;
  bool clovis_is_read_verify;
  unsigned int clovis_tm_recv_queue_min_len;
  unsigned int clovis_max_rpc_msg_size;
  std::string clovis_process_fid;
  int clovis_idx_service_id;
  std::string clovis_cass_cluster_ep;
  std::string clovis_cass_keyspace;
  int clovis_cass_max_column_family_num;
  size_t clovis_read_pool_initial_buffer_count;
  size_t clovis_read_pool_expandable_count;
  size_t clovis_read_pool_max_threshold;

  size_t libevent_pool_initial_size;
  size_t libevent_pool_expandable_size;
  size_t libevent_pool_max_threshold;
  size_t libevent_pool_buffer_size;
  size_t libevent_max_read_size;

  std::string s3_daemon_dir;
  unsigned short s3_daemon_redirect;

  bool stats_enable;
  std::string statsd_ip_addr;
  unsigned short statsd_port;
  unsigned short statsd_max_send_retry;
  std::string stats_whitelist_filename;

  evbase_t* eventbase;

  static S3Option* option_instance;
  void set_clovis_idx_fetch_count(short count);

  S3Option() {
    cmd_opt_flag = 0;

    s3_bind_addr = FLAGS_s3host;
    s3_bind_port = FLAGS_s3port;
    s3_pidfile = "/var/run/s3server.pid";

    read_ahead_multiple = 1;

    s3_default_endpoint = "s3.seagate.com";
    s3_region_endpoints.insert("s3-us.seagate.com");
    s3_region_endpoints.insert("s3-europe.seagate.com");
    s3_region_endpoints.insert("s3-asia.seagate.com");
    s3_iam_cert_file = "/opt/seagate/auth/resources/iam.seagate.com.crt";
    s3_grace_period_sec = 10;  // 10 seconds
    is_s3_shutting_down = false;

    log_dir = "/var/log/seagate/s3";
    log_level = FLAGS_s3loglevel;
    log_file_max_size_mb = 100;  // 100 MB
    log_buffering_enable = true;
    log_flush_frequency_sec = 30;  // 30 seconds

    clovis_layout_id = FLAGS_clovislayoutid;
    clovis_local_addr = FLAGS_clovislocal;
    clovis_ha_addr = FLAGS_clovisha;
    clovis_profile = FLAGS_clovisprofilefid;
    clovis_is_oostore = false;
    clovis_is_read_verify = false;
    clovis_tm_recv_queue_min_len = 2;
    clovis_max_rpc_msg_size = 131072;
    clovis_process_fid = FLAGS_clovisprocessfid;
    clovis_idx_service_id = 2;
    clovis_cass_cluster_ep = "127.0.0.1";
    clovis_cass_keyspace = "clovis_index_keyspace";
    clovis_cass_max_column_family_num = 1;

    // libevent_pool_buffer_size is used for each item in this
    clovis_read_pool_initial_buffer_count = 10;   // 10 buffer
    clovis_read_pool_expandable_count = 1048576;  // 1mb
    clovis_read_pool_max_threshold = 104857600;  // 100mb

    libevent_pool_buffer_size = 4096;
    libevent_pool_initial_size = 10485760;
    libevent_pool_expandable_size = 1048576;
    libevent_pool_max_threshold = 104857600;
    libevent_max_read_size = 16384;  // libevent max

    auth_ip_addr = FLAGS_authhost;
    auth_port = FLAGS_authport;

    option_file = "/opt/seagate/s3/conf/s3config.yaml";
    layout_recommendation_file =
        "/opt/seagate/s3/conf/s3_obj_layout_mapping.yaml";

    s3_daemon_dir = "/";
    s3_daemon_redirect = 1;

    perf_enabled = FLAGS_perfenable;
    perf_log_file = FLAGS_perflogfile;

    clovis_units_per_request = 1;
    clovis_idx_fetch_count = 100;

    retry_interval_millisec = 0;
    max_retry_count = 0;

    stats_enable = false;
    statsd_ip_addr = FLAGS_statsd_host;
    statsd_port = FLAGS_statsd_port;
    statsd_max_send_retry = 3;
    stats_whitelist_filename = "/opt/seagate/s3/conf/s3stats-whitelist.yaml";

    eventbase = NULL;

    // find out the nodename
    char hostname[HOST_NAME_MAX];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
      struct addrinfo hints;
      struct addrinfo* result;
      memset(&hints, 0, sizeof(struct addrinfo));
      hints.ai_family = AF_UNSPEC;
      hints.ai_flags = AI_CANONNAME;
      if (getaddrinfo(hostname, NULL, &hints, &result) == 0) {
        // yay!, we got the FQDN!
        s3_nodename = std::string(result->ai_canonname);
        freeaddrinfo(result);
      } else {
        // !(yay), be happy with just the hostname
        s3_nodename = std::string(hostname);
      }
    }
  }

 public:
  bool load_section(std::string section_name, bool force_override_from_config);
  bool load_all_sections(bool force_override_from_config);

  std::string get_s3_nodename();
  std::string get_bind_addr();
  std::string get_s3_pidfile();
  unsigned short get_s3_bind_port();

  int get_read_ahead_multiple();
  std::string get_default_endpoint();
  std::set<std::string>& get_region_endpoints();
  unsigned short get_s3_grace_period_sec();
  bool get_is_s3_shutting_down();
  void set_is_s3_shutting_down(bool is_shutting_down);

  std::string get_auth_ip_addr();
  unsigned short get_auth_port();
  void disable_auth();
  void enable_auth();
  bool is_auth_disabled();

  std::string get_option_file();
  void set_option_file(std::string filename);

  std::string get_layout_recommendation_file();
  void set_layout_recommendation_file(std::string filename);

  std::string get_daemon_dir();
  unsigned short do_redirection();
  void set_daemon_dir(std::string path);
  void set_redirection(unsigned short redirect);

  std::string get_log_dir();
  std::string get_log_level();
  int get_log_file_max_size_in_mb();
  bool is_s3_ssl_auth_enabled();
  bool is_s3_reuseport_enabled();
  const char* get_iam_cert_file();
  bool is_log_buffering_enabled();
  bool is_murmurhash_oid_enabled();
  int get_log_flush_frequency_in_sec();

  unsigned short s3_performance_enabled();
  std::string get_perf_log_filename();

  std::string get_clovis_local_addr();
  std::string get_clovis_ha_addr();
  std::string get_clovis_prof();
  unsigned short get_clovis_layout_id();
  std::vector<int> get_clovis_unit_sizes_for_mem_pool();
  unsigned short get_clovis_units_per_request();
  unsigned int get_clovis_write_payload_size(int layoutid);
  unsigned int get_clovis_read_payload_size(int layoutid);
  int get_clovis_idx_fetch_count();
  unsigned short get_max_retry_count();
  unsigned short get_retry_interval_in_millisec();
  size_t get_clovis_read_pool_initial_buffer_count();
  size_t get_clovis_read_pool_expandable_count();
  size_t get_clovis_read_pool_max_threshold();

  size_t get_libevent_pool_initial_size();
  size_t get_libevent_pool_expandable_size();
  size_t get_libevent_pool_max_threshold();
  size_t get_libevent_pool_buffer_size();
  size_t get_libevent_max_read_size();

  bool get_clovis_is_oostore();
  bool get_clovis_is_read_verify();
  unsigned int get_clovis_tm_recv_queue_min_len();
  unsigned int get_clovis_max_rpc_msg_size();
  std::string get_clovis_process_fid();
  int get_clovis_idx_service_id();
  std::string get_clovis_cass_cluster_ep();
  std::string get_clovis_cass_keyspace();
  int get_clovis_cass_max_column_family_num();

  void set_cmdline_option(int option_flag, const char* option);
  int get_cmd_opt_flag();

  // Check if any fake out options are provided.
  bool is_fake_clovis_createobj();
  bool is_fake_clovis_writeobj();
  bool is_fake_clovis_deleteobj();
  bool is_fake_clovis_createidx();
  bool is_fake_clovis_deleteidx();
  bool is_fake_clovis_getkv();
  bool is_fake_clovis_putkv();
  bool is_fake_clovis_deletekv();

  void set_eventbase(evbase_t* base);
  evbase_t* get_eventbase();

  bool is_stats_enabled();
  void set_stats_enable(bool enable);
  std::string get_statsd_ip_addr();
  unsigned short get_statsd_port();
  unsigned short get_statsd_max_send_retry();
  std::string get_stats_whitelist_filename();
  void set_stats_whitelist_filename(std::string filename);

  // Fault injection Option
  void enable_fault_injection();
  bool is_fi_enabled();
  void enable_reuseport();
  void enable_murmurhash_oid();
  void disable_murmurhash_oid();

  void dump_options();

  static S3Option* get_instance() {
    if (!option_instance) {
      option_instance = new S3Option();
    }
    return option_instance;
  }

  static void destroy_instance() {
    if (option_instance) {
      delete option_instance;
      option_instance = NULL;
    }
  }
  FRIEND_TEST(S3DeleteBucketActionTest,
              FetchMultipartObjectSuccessMaxFetchCountLessThanMapSize);
  FRIEND_TEST(S3GetMultipartPartActionTest,
              GetNextObjectsSuccessfulListNotTruncated);
  FRIEND_TEST(S3GetMultipartPartActionTest,
              GetNextObjectsSuccessfulGetMoreObjects);
};
#endif
