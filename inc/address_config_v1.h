#ifndef __ADDRESS_CONFIG_H_V1__
#define __ADDRESS_CONFIG_H_V1__

#include "../inc/specs.h"
#include "../inc/memory.h"

/*=========  ConvConfig  ==========*/
constexpr int kConvConfig_AlexNet_imgz_conv1_addr {0};
constexpr int kConvConfig_AlexNet_imgz_conv2_addr {1};
constexpr int kConvConfig_AlexNet_imgz_conv3_addr {2};
constexpr int kConvConfig_AlexNet_imgz_conv4_addr {3};
constexpr int kConvConfig_AlexNet_imgz_conv5_addr {4};
constexpr int kConvConfig_AlexNet_imgz_conv6_addr {5};
constexpr int kConvConfig_c_z_k_addr              {6};
constexpr int kConvConfig_r_z_k_addr              {7};
constexpr int kConvConfig_AlexNet_imgx_conv1_addr {8};
constexpr int kConvConfig_AlexNet_imgx_conv2_addr {9};
constexpr int kConvConfig_AlexNet_imgx_conv3_addr {10};
constexpr int kConvConfig_AlexNet_imgx_conv4_addr {11};
constexpr int kConvConfig_AlexNet_imgx_conv5_addr {12};
constexpr int kConvConfig_AlexNet_imgx_conv6_addr {13};
constexpr int kConvConfig_c_x_addr                {14};
constexpr int kConvConfig_r_x_addr                {15};

constexpr int kConvConfig_c_z_addr                {16};
constexpr int kConvConfig_cls_conv1_addr          {17};
constexpr int kConvConfig_cls_conv2_addr          {18};
constexpr int kConvConfig_cls_conv3_addr          {19};
constexpr int kConvConfig_cls_score_addr          {20};
constexpr int kConvConfig_ctr_score_addr          {21};

constexpr int kConvConfig_r_z_addr                {22};
constexpr int kConvConfig_bbox_conv1_addr         {23};
constexpr int kConvConfig_bbox_conv2_addr         {24};
constexpr int kConvConfig_bbox_conv3_addr         {25};
constexpr int kConvConfig_offset_score_addr       {26};



/*=========  Input Image  ==========*/

constexpr int kimg_z_size                  {127};
constexpr int kimg_z_depth                 {3};
constexpr int kimg_z_addr_depth_start      {0};
constexpr int kimg_z_addr_depth_end        {1+kimg_z_addr_depth_start+kimg_z_size*kimg_z_size/kDramAccessBytes};
                                            //1+127*127/16 = 1009 rows
constexpr int kimg_z_addr_data_start       {kimg_z_addr_depth_end};
constexpr int kimg_z_addr_interval_of_data {1};
constexpr int kimg_z_addr_end              {kimg_z_addr_data_start+1+kimg_z_size*kimg_z_size*kimg_z_addr_interval_of_data/2};

constexpr int kimg_x_size                  {303};
constexpr int kimg_x_depth                 {3};
constexpr int kimg_x_addr_depth_start      {kimg_z_addr_end};
constexpr int kimg_x_addr_depth_end        {1+kimg_x_addr_depth_start+kimg_x_size*kimg_x_size/kDramAccessBytes};
constexpr int kimg_x_addr_data_start       {kimg_x_addr_depth_end};
constexpr int kimg_x_addr_interval_of_data {1};
constexpr int kimg_x_addr_end              {kimg_x_addr_data_start+1+kimg_x_size*kimg_x_size*kimg_x_addr_interval_of_data/2};

/*=========  Output Feature Map  ==========*/

constexpr int koa_ft_map_addr_depth_start       {kimg_x_addr_end};
constexpr int koa_ft_map_addr_depth_end         {1+koa_ft_map_addr_depth_start+151*151/kDramAccessBytes};
                                                //1426 rows
constexpr int koa_ft_map_addr_data_start        {koa_ft_map_addr_depth_end};
constexpr int koa_ft_map_addr_interval_of_data  {2*specs::kIaBundleMaxSize/kDramAccessBytes};
                                                // 32 rows

constexpr int koa_ft_map_addr_end               {1+koa_ft_map_addr_data_start+149*149*128/(kDramAccessBytes/2)};
                                                //273612 rows

/*=========  Result  ==========*/


constexpr int kresult_cls_score_start           {koa_ft_map_addr_end};
constexpr int kresult_cls_score_end             {1+kresult_cls_score_start+17*17*4/(kDramAccessBytes/2)};

constexpr int kresult_ctr_score_start           {kresult_cls_score_end};
constexpr int kresult_ctr_score_end             {1+kresult_ctr_score_start+17*17*4/(kDramAccessBytes/2)};

constexpr int kresult_offset_score_start        {kresult_ctr_score_end};
constexpr int kresult_offset_score_end          {1+kresult_offset_score_start+17*17*4/(kDramAccessBytes/2)};


/*=========  c_x, r_x  ==========*/

constexpr int koa_ft_map_c_x_addr_depth_start       {kresult_offset_score_end};
constexpr int koa_ft_map_c_x_addr_depth_end         {koa_ft_map_c_x_addr_depth_start+28*28/kDramAccessBytes};
constexpr int koa_ft_map_c_x_addr_data_start        {koa_ft_map_c_x_addr_depth_end};
constexpr int koa_ft_map_c_x_addr_interval_of_data  {2*256/kDramAccessBytes};
constexpr int koa_ft_map_c_x_addr_end               {koa_ft_map_c_x_addr_data_start+28*28*koa_ft_map_c_x_addr_interval_of_data};


constexpr int koa_ft_map_r_x_addr_depth_start       {koa_ft_map_c_x_addr_end};
constexpr int koa_ft_map_r_x_addr_depth_end         {koa_ft_map_r_x_addr_depth_start+28*28/kDramAccessBytes};
constexpr int koa_ft_map_r_x_addr_data_start        {koa_ft_map_r_x_addr_depth_end};
constexpr int koa_ft_map_r_x_addr_interval_of_data  {2*256/kDramAccessBytes};
constexpr int koa_ft_map_r_x_addr_end               {koa_ft_map_r_x_addr_data_start+28*28*koa_ft_map_r_x_addr_interval_of_data};


/*=========  Z Kernel  ==========*/

constexpr int kc_z_size                     {6};
constexpr int kc_z_depth                    {0};
constexpr int kc_z_addr_depth_start         {koa_ft_map_r_x_addr_end};
constexpr int kc_z_addr_depth_end           {kc_z_addr_depth_start+kc_z_size};
                                            //1+127*127/16 = 1009 rows
constexpr int kc_z_addr_data_start          {kc_z_addr_depth_end};
constexpr int kc_z_addr_interval_of_data    {2*specs::kWBundleMaxSize/kDramAccessBytes};
constexpr int kc_z_addr_end                 {kc_z_addr_data_start+kc_z_size*kc_z_size*kc_z_addr_interval_of_data};

constexpr int kr_z_size                     {6};
constexpr int kr_z_depth                    {0};
constexpr int kr_z_addr_depth_start         {kc_z_addr_end};
constexpr int kr_z_addr_depth_end           {kr_z_addr_depth_start+kr_z_size};
                                            //1+127*127/16 = 1009 rows
constexpr int kr_z_addr_data_start          {kr_z_addr_depth_end};
constexpr int kr_z_addr_interval_of_data    {2*specs::kWBundleMaxSize/kDramAccessBytes};
constexpr int kr_z_addr_end                 {kr_z_addr_data_start+kr_z_size*kr_z_size*kr_z_addr_interval_of_data};

/*=========  Weight          ==========*/
// constexpr int kM_addr_start                 {kr_z_addr_end};
// constexpr int kM_addr_end                   {kM_addr_start+2};


/*=========  Weight Prepare  ==========*/
constexpr int kconv1_kernel_size               {3};
constexpr int kconv1_kernel_depth              {3};
constexpr int kconv1_kernel_num                {96};
constexpr int kconv1_addr_interval_of_data     {32};
constexpr int kconv1_addr_interval_of_prk      {18};
constexpr int kconv1_bundle_num                {12};
constexpr int kconv1_addr_info_range           {1};



constexpr int kconv2_kernel_size            {3};
constexpr int kconv2_kernel_depth           {96};
constexpr int kconv2_kernel_num             {128};
constexpr int kconv2_addr_interval_of_data  {32};
constexpr int kconv2_addr_interval_of_prk   {1};
constexpr int kconv2_bundle_num             {429};
constexpr int kconv2_addr_info_range        {27};



constexpr int kconv3_kernel_size            {3};
constexpr int kconv3_kernel_depth           {128};
constexpr int kconv3_kernel_num             {160};
constexpr int kconv3_addr_interval_of_data  {32};
constexpr int kconv3_addr_interval_of_prk   {1};
constexpr int kconv3_bundle_num             {708};
constexpr int kconv3_addr_info_range        {45};



constexpr int kconv4_kernel_size            {3};
constexpr int kconv4_kernel_depth           {160};
constexpr int kconv4_kernel_num             {192};
constexpr int kconv4_addr_interval_of_data  {32};
constexpr int kconv4_addr_interval_of_prk   {1};
constexpr int kconv4_bundle_num             {1055};
constexpr int kconv4_addr_info_range        {66};



constexpr int kconv5_kernel_size            {3};
constexpr int kconv5_kernel_depth           {192};
constexpr int kconv5_kernel_num             {224};
constexpr int kconv5_addr_interval_of_data  {32};
constexpr int kconv5_addr_interval_of_prk   {1};
constexpr int kconv5_bundle_num             {1478};
constexpr int kconv5_addr_info_range        {93};



constexpr int kconv6_kernel_size            {3};
constexpr int kconv6_kernel_depth           {224};
constexpr int kconv6_kernel_num             {256};
constexpr int kconv6_addr_interval_of_data  {32};
constexpr int kconv6_addr_interval_of_prk   {1};
constexpr int kconv6_bundle_num             {2010};
constexpr int kconv6_addr_info_range        {126};


constexpr int kc_z_k_kernel_size               {3};
constexpr int kc_z_k_kernel_depth              {256};
constexpr int kc_z_k_kernel_num                {256};
constexpr int kc_z_k_addr_interval_of_data     {32};
constexpr int kc_z_k_addr_interval_of_prk      {1};
constexpr int kc_z_k_bundle_num                {2228};
constexpr int kc_z_k_addr_info_range           {140};


constexpr int kr_z_k_kernel_size               {3};
constexpr int kr_z_k_kernel_depth              {256};
constexpr int kr_z_k_kernel_num                {256};
constexpr int kr_z_k_addr_interval_of_data     {32};
constexpr int kr_z_k_addr_interval_of_prk      {1};
constexpr int kr_z_k_bundle_num                {2222};
constexpr int kr_z_k_addr_info_range           {139};


constexpr int kc_x_kernel_size                  {3};
constexpr int kc_x_kernel_depth                 {256};
constexpr int kc_x_kernel_num                   {256};
constexpr int kc_x_addr_interval_of_data        {32};
constexpr int kc_x_addr_interval_of_prk         {1};
constexpr int kc_x_bundle_num                   {2219};
constexpr int kc_x_addr_info_range              {139};


constexpr int kr_x_kernel_size                  {3};
constexpr int kr_x_kernel_depth                 {256};
constexpr int kr_x_kernel_num                   {256};
constexpr int kr_x_addr_interval_of_data        {32};
constexpr int kr_x_addr_interval_of_prk         {1};
constexpr int kr_x_bundle_num                   {2204};
constexpr int kr_x_addr_info_range              {138};


constexpr int kcls_conv1_kernel_size            {3};
constexpr int kcls_conv1_kernel_depth           {256};
constexpr int kcls_conv1_kernel_num             {256};
constexpr int kcls_conv1_addr_interval_of_data  {32};
constexpr int kcls_conv1_addr_interval_of_prk   {1};
constexpr int kcls_conv1_bundle_num             {2204};
constexpr int kcls_conv1_addr_info_range        {138};


constexpr int kcls_conv2_kernel_size            {3};
constexpr int kcls_conv2_kernel_depth           {256};
constexpr int kcls_conv2_kernel_num             {256};
constexpr int kcls_conv2_addr_interval_of_data  {32};
constexpr int kcls_conv2_addr_interval_of_prk   {1};
constexpr int kcls_conv2_bundle_num             {2085};
constexpr int kcls_conv2_addr_info_range        {131};


constexpr int kcls_conv3_kernel_size            {3};
constexpr int kcls_conv3_kernel_depth           {256};
constexpr int kcls_conv3_kernel_num             {256};
constexpr int kcls_conv3_addr_interval_of_data  {32};
constexpr int kcls_conv3_addr_interval_of_prk   {1};
constexpr int kcls_conv3_bundle_num             {2193};
constexpr int kcls_conv3_addr_info_range        {138};


constexpr int kbbox_conv1_kernel_size            {3};
constexpr int kbbox_conv1_kernel_depth           {256};
constexpr int kbbox_conv1_kernel_num             {256};
constexpr int kbbox_conv1_addr_interval_of_data  {32};
constexpr int kbbox_conv1_addr_interval_of_prk   {1};
constexpr int kbbox_conv1_bundle_num             {2072};
constexpr int kbbox_conv1_addr_info_range        {130};


constexpr int kbbox_conv2_kernel_size            {3};
constexpr int kbbox_conv2_kernel_depth           {256};
constexpr int kbbox_conv2_kernel_num             {256};
constexpr int kbbox_conv2_addr_interval_of_data  {32};
constexpr int kbbox_conv2_addr_interval_of_prk   {1};
constexpr int kbbox_conv2_bundle_num             {1077};
constexpr int kbbox_conv2_addr_info_range        {68};


constexpr int kbbox_conv3_kernel_size            {3};
constexpr int kbbox_conv3_kernel_depth           {256};
constexpr int kbbox_conv3_kernel_num             {256};
constexpr int kbbox_conv3_addr_interval_of_data  {32};
constexpr int kbbox_conv3_addr_interval_of_prk   {1};
constexpr int kbbox_conv3_bundle_num             {1852};
constexpr int kbbox_conv3_addr_info_range        {116};


constexpr int kcls_score_kernel_size               {3};
constexpr int kcls_score_kernel_depth              {256};
constexpr int kcls_score_kernel_num                {1};
constexpr int kcls_score_addr_interval_of_data     {32};
constexpr int kcls_score_addr_interval_of_prk      {1};
constexpr int kcls_score_bundle_num                {2};
constexpr int kcls_score_addr_info_range           {1};


constexpr int kctr_score_kernel_size               {3};
constexpr int kctr_score_kernel_depth              {256};
constexpr int kctr_score_kernel_num                {1};
constexpr int kctr_score_addr_interval_of_data     {32};
constexpr int kctr_score_addr_interval_of_prk      {1};
constexpr int kctr_score_bundle_num                {1};
constexpr int kctr_score_addr_info_range           {1};


constexpr int koffset_score_kernel_size            {3};
constexpr int koffset_score_kernel_depth           {256};
constexpr int koffset_score_kernel_num             {4};
constexpr int koffset_score_addr_interval_of_data  {32};
constexpr int koffset_score_addr_interval_of_prk   {1};
constexpr int koffset_score_bundle_num             {4};
constexpr int koffset_score_addr_info_range        {1};


/*=========  Weight Address  ==========*/

constexpr int kconv1_addr_s_start           {kr_z_addr_end};
constexpr int kconv1_addr_s_end             {kconv1_addr_s_start+kconv1_addr_info_range};
constexpr int kconv1_addr_depth_start       {kconv1_addr_s_end};
constexpr int kconv1_addr_depth_end         {kconv1_addr_depth_start+kconv1_addr_info_range};
constexpr int kconv1_addr_prkdepth_start    {kconv1_addr_depth_end};
constexpr int kconv1_addr_prkdepth_end      {kconv1_addr_prkdepth_start+kconv1_addr_info_range};
constexpr int kconv1_addr_data_start        {kconv1_addr_prkdepth_end};
constexpr int kconv1_addr_data_end          {kconv1_addr_data_start + kconv1_bundle_num*kconv1_addr_interval_of_data};
constexpr int kconv1_addr_prk_start         {kconv1_addr_data_end};
constexpr int kconv1_addr_prk_end           {kconv1_addr_prk_start + kconv1_bundle_num*kconv1_addr_interval_of_prk};
constexpr int kconv1_addr_bias_start        {kconv1_addr_prk_end};
constexpr int kconv1_addr_bias_end          {kconv1_addr_bias_start + kconv1_kernel_num/(kDramAccessBytes/2)};
constexpr int kconv1_addr_end               {kconv1_addr_bias_end};



constexpr int kconv2_addr_s_start           {kconv1_addr_end};
constexpr int kconv2_addr_s_end             {kconv2_addr_s_start+kconv2_addr_info_range};
constexpr int kconv2_addr_depth_start       {kconv2_addr_s_end};
constexpr int kconv2_addr_depth_end         {kconv2_addr_depth_start+kconv2_addr_info_range};
constexpr int kconv2_addr_prkdepth_start    {kconv2_addr_depth_end};
constexpr int kconv2_addr_prkdepth_end      {kconv2_addr_prkdepth_start+kconv2_addr_info_range};
constexpr int kconv2_addr_data_start        {kconv2_addr_prkdepth_end};
constexpr int kconv2_addr_data_end          {kconv2_addr_data_start + kconv2_bundle_num*kconv2_addr_interval_of_data};
constexpr int kconv2_addr_prk_start         {kconv2_addr_data_end};
constexpr int kconv2_addr_prk_end           {kconv2_addr_prk_start + kconv2_bundle_num*kconv2_addr_interval_of_prk};
constexpr int kconv2_addr_bias_start        {kconv2_addr_prk_end};
constexpr int kconv2_addr_bias_end          {kconv2_addr_bias_start + kconv2_kernel_num/(kDramAccessBytes/2)};
constexpr int kconv2_addr_end               {kconv2_addr_bias_end};



constexpr int kconv3_addr_s_start           {kconv2_addr_end};
constexpr int kconv3_addr_s_end             {kconv3_addr_s_start+kconv3_addr_info_range};
constexpr int kconv3_addr_depth_start       {kconv3_addr_s_end};
constexpr int kconv3_addr_depth_end         {kconv3_addr_depth_start+kconv3_addr_info_range};
constexpr int kconv3_addr_prkdepth_start    {kconv3_addr_depth_end};
constexpr int kconv3_addr_prkdepth_end      {kconv3_addr_prkdepth_start+kconv3_addr_info_range};
constexpr int kconv3_addr_data_start        {kconv3_addr_prkdepth_end};
constexpr int kconv3_addr_data_end          {kconv3_addr_data_start + kconv3_bundle_num*kconv3_addr_interval_of_data};
constexpr int kconv3_addr_prk_start         {kconv3_addr_data_end};
constexpr int kconv3_addr_prk_end           {kconv3_addr_prk_start + kconv3_bundle_num*kconv3_addr_interval_of_prk};
constexpr int kconv3_addr_bias_start        {kconv3_addr_prk_end};
constexpr int kconv3_addr_bias_end          {kconv3_addr_bias_start + kconv3_kernel_num/(kDramAccessBytes/2)};
constexpr int kconv3_addr_end               {kconv3_addr_bias_end};



constexpr int kconv4_addr_s_start           {kconv3_addr_end};
constexpr int kconv4_addr_s_end             {kconv4_addr_s_start+kconv4_addr_info_range};
constexpr int kconv4_addr_depth_start       {kconv4_addr_s_end};
constexpr int kconv4_addr_depth_end         {kconv4_addr_depth_start+kconv4_addr_info_range};
constexpr int kconv4_addr_prkdepth_start    {kconv4_addr_depth_end};
constexpr int kconv4_addr_prkdepth_end      {kconv4_addr_prkdepth_start+kconv4_addr_info_range};
constexpr int kconv4_addr_data_start        {kconv4_addr_prkdepth_end};
constexpr int kconv4_addr_data_end          {kconv4_addr_data_start + kconv4_bundle_num*kconv4_addr_interval_of_data};
constexpr int kconv4_addr_prk_start         {kconv4_addr_data_end};
constexpr int kconv4_addr_prk_end           {kconv4_addr_prk_start + kconv4_bundle_num*kconv4_addr_interval_of_prk};
constexpr int kconv4_addr_bias_start        {kconv4_addr_prk_end};
constexpr int kconv4_addr_bias_end          {kconv4_addr_bias_start + kconv4_kernel_num/(kDramAccessBytes/2)};
constexpr int kconv4_addr_end               {kconv4_addr_bias_end};



constexpr int kconv5_addr_s_start           {kconv4_addr_end};
constexpr int kconv5_addr_s_end             {kconv5_addr_s_start+kconv5_addr_info_range};
constexpr int kconv5_addr_depth_start       {kconv5_addr_s_end};
constexpr int kconv5_addr_depth_end         {kconv5_addr_depth_start+kconv5_addr_info_range};
constexpr int kconv5_addr_prkdepth_start    {kconv5_addr_depth_end};
constexpr int kconv5_addr_prkdepth_end      {kconv5_addr_prkdepth_start+kconv5_addr_info_range};
constexpr int kconv5_addr_data_start        {kconv5_addr_prkdepth_end};
constexpr int kconv5_addr_data_end          {kconv5_addr_data_start+kconv5_bundle_num*kconv5_addr_interval_of_data};
constexpr int kconv5_addr_prk_start         {kconv5_addr_data_end};
constexpr int kconv5_addr_prk_end           {kconv5_addr_prk_start + kconv5_bundle_num*kconv5_addr_interval_of_prk};
constexpr int kconv5_addr_bias_start        {kconv5_addr_prk_end};
constexpr int kconv5_addr_bias_end          {kconv5_addr_bias_start + kconv5_kernel_num/(kDramAccessBytes/2)};
constexpr int kconv5_addr_end               {kconv5_addr_bias_end};



constexpr int kconv6_addr_s_start           {kconv5_addr_end};
constexpr int kconv6_addr_s_end             {kconv6_addr_s_start+kconv6_addr_info_range};
constexpr int kconv6_addr_depth_start       {kconv6_addr_s_end};
constexpr int kconv6_addr_depth_end         {kconv6_addr_depth_start+kconv6_addr_info_range};
constexpr int kconv6_addr_prkdepth_start    {kconv6_addr_depth_end};
constexpr int kconv6_addr_prkdepth_end      {kconv6_addr_prkdepth_start+kconv6_addr_info_range};
constexpr int kconv6_addr_data_start        {kconv6_addr_prkdepth_end};
constexpr int kconv6_addr_data_end          {kconv6_addr_data_start+kconv6_bundle_num*kconv6_addr_interval_of_data};
constexpr int kconv6_addr_prk_start         {kconv6_addr_data_end};
constexpr int kconv6_addr_prk_end           {kconv6_addr_prk_start + kconv6_bundle_num*kconv6_addr_interval_of_prk};
constexpr int kconv6_addr_bias_start        {kconv6_addr_prk_end};
constexpr int kconv6_addr_bias_end          {kconv6_addr_bias_start + kconv6_kernel_num/(kDramAccessBytes/2)};
constexpr int kconv6_addr_end               {kconv6_addr_bias_end};

/*=========  Weight: Feature Adjustment  ==========*/


constexpr int kc_z_k_addr_s_start           {kconv6_addr_end};
constexpr int kc_z_k_addr_s_end             {kc_z_k_addr_s_start+kc_z_k_addr_info_range};
constexpr int kc_z_k_addr_depth_start       {kc_z_k_addr_s_end};
constexpr int kc_z_k_addr_depth_end         {kc_z_k_addr_depth_start+kc_z_k_addr_info_range};
constexpr int kc_z_k_addr_prkdepth_start    {kc_z_k_addr_depth_end};
constexpr int kc_z_k_addr_prkdepth_end      {kc_z_k_addr_prkdepth_start+kc_z_k_addr_info_range};
constexpr int kc_z_k_addr_data_start        {kc_z_k_addr_prkdepth_end};
constexpr int kc_z_k_addr_data_end          {kc_z_k_addr_data_start+kc_z_k_bundle_num*kc_z_k_addr_interval_of_data};
constexpr int kc_z_k_addr_prk_start         {kc_z_k_addr_data_end};
constexpr int kc_z_k_addr_prk_end           {kc_z_k_addr_prk_start + kc_z_k_bundle_num*kc_z_k_addr_interval_of_prk};
constexpr int kc_z_k_addr_bias_start        {kc_z_k_addr_prk_end};
constexpr int kc_z_k_addr_bias_end          {kc_z_k_addr_bias_start + kc_z_k_kernel_num/(kDramAccessBytes/2)};
constexpr int kc_z_k_addr_end               {kc_z_k_addr_bias_end};



constexpr int kr_z_k_addr_s_start           {kc_z_k_addr_end};
constexpr int kr_z_k_addr_s_end             {kr_z_k_addr_s_start+kr_z_k_addr_info_range};
constexpr int kr_z_k_addr_depth_start       {kr_z_k_addr_s_end};
constexpr int kr_z_k_addr_depth_end         {kr_z_k_addr_depth_start+kr_z_k_addr_info_range};
constexpr int kr_z_k_addr_prkdepth_start    {kr_z_k_addr_depth_end};
constexpr int kr_z_k_addr_prkdepth_end      {kr_z_k_addr_prkdepth_start+kr_z_k_addr_info_range};
constexpr int kr_z_k_addr_data_start        {kr_z_k_addr_prkdepth_end};
constexpr int kr_z_k_addr_data_end          {kr_z_k_addr_data_start+kr_z_k_bundle_num*kr_z_k_addr_interval_of_data};
constexpr int kr_z_k_addr_prk_start         {kr_z_k_addr_data_end};
constexpr int kr_z_k_addr_prk_end           {kr_z_k_addr_prk_start + kr_z_k_bundle_num*kr_z_k_addr_interval_of_prk};
constexpr int kr_z_k_addr_bias_start        {kr_z_k_addr_prk_end};
constexpr int kr_z_k_addr_bias_end          {kr_z_k_addr_bias_start + kr_z_k_kernel_num/(kDramAccessBytes/2)};
constexpr int kr_z_k_addr_end               {kr_z_k_addr_bias_end};



constexpr int kc_x_addr_s_start           {kr_z_k_addr_end};
constexpr int kc_x_addr_s_end             {kc_x_addr_s_start+kc_x_addr_info_range};
constexpr int kc_x_addr_depth_start       {kc_x_addr_s_end};
constexpr int kc_x_addr_depth_end         {kc_x_addr_depth_start+kc_x_addr_info_range};
constexpr int kc_x_addr_prkdepth_start    {kc_x_addr_depth_end};
constexpr int kc_x_addr_prkdepth_end      {kc_x_addr_prkdepth_start+kc_x_addr_info_range};
constexpr int kc_x_addr_data_start        {kc_x_addr_prkdepth_end};
constexpr int kc_x_addr_data_end          {kc_x_addr_data_start+kc_x_bundle_num*kc_x_addr_interval_of_data};
constexpr int kc_x_addr_prk_start         {kc_x_addr_data_end};
constexpr int kc_x_addr_prk_end           {kc_x_addr_prk_start + kc_x_bundle_num*kc_x_addr_interval_of_prk};
constexpr int kc_x_addr_bias_start        {kc_x_addr_prk_end};
constexpr int kc_x_addr_bias_end          {kc_x_addr_bias_start + kc_x_kernel_num/(kDramAccessBytes/2)};
constexpr int kc_x_addr_end               {kc_x_addr_bias_end};


constexpr int kr_x_addr_s_start           {kc_x_addr_end};
constexpr int kr_x_addr_s_end             {kr_x_addr_s_start+kr_x_addr_info_range};
constexpr int kr_x_addr_depth_start       {kr_x_addr_s_end};
constexpr int kr_x_addr_depth_end         {kr_x_addr_depth_start+kr_x_addr_info_range};
constexpr int kr_x_addr_prkdepth_start    {kr_x_addr_depth_end};
constexpr int kr_x_addr_prkdepth_end      {kr_x_addr_prkdepth_start+kr_x_addr_info_range};
constexpr int kr_x_addr_data_start        {kr_x_addr_prkdepth_end};
constexpr int kr_x_addr_data_end          {kr_x_addr_data_start+kr_x_bundle_num*kr_x_addr_interval_of_data};
constexpr int kr_x_addr_prk_start         {kr_x_addr_data_end};
constexpr int kr_x_addr_prk_end           {kr_x_addr_prk_start + kr_x_bundle_num*kr_x_addr_interval_of_prk};
constexpr int kr_x_addr_bias_start        {kr_x_addr_prk_end};
constexpr int kr_x_addr_bias_end          {kr_x_addr_bias_start + kr_x_kernel_num/(kDramAccessBytes/2)};
constexpr int kr_x_addr_end               {kr_x_addr_bias_end};


/*=========  Weight: Head  ==========*/

//cls

constexpr int kcls_conv1_addr_s_start           {kr_x_addr_end};
constexpr int kcls_conv1_addr_s_end             {kcls_conv1_addr_s_start+kcls_conv1_addr_info_range};
constexpr int kcls_conv1_addr_depth_start       {kcls_conv1_addr_s_end};
constexpr int kcls_conv1_addr_depth_end         {kcls_conv1_addr_depth_start+kcls_conv1_addr_info_range};
constexpr int kcls_conv1_addr_prkdepth_start    {kcls_conv1_addr_depth_end};
constexpr int kcls_conv1_addr_prkdepth_end      {kcls_conv1_addr_prkdepth_start+kcls_conv1_addr_info_range};
constexpr int kcls_conv1_addr_data_start        {kcls_conv1_addr_prkdepth_end};
constexpr int kcls_conv1_addr_data_end          {kcls_conv1_addr_data_start+kcls_conv1_bundle_num*kcls_conv1_addr_interval_of_data};
constexpr int kcls_conv1_addr_prk_start         {kcls_conv1_addr_data_end};
constexpr int kcls_conv1_addr_prk_end           {kcls_conv1_addr_prk_start + kcls_conv1_bundle_num*kcls_conv1_addr_interval_of_prk};
constexpr int kcls_conv1_addr_bias_start        {kcls_conv1_addr_prk_end};
constexpr int kcls_conv1_addr_bias_end          {kcls_conv1_addr_bias_start + kcls_conv1_kernel_num/(kDramAccessBytes/2)};
constexpr int kcls_conv1_addr_end               {kcls_conv1_addr_bias_end};



constexpr int kcls_conv2_addr_s_start           {kcls_conv1_addr_end};
constexpr int kcls_conv2_addr_s_end             {kcls_conv2_addr_s_start+kcls_conv2_addr_info_range};
constexpr int kcls_conv2_addr_depth_start       {kcls_conv2_addr_s_end};
constexpr int kcls_conv2_addr_depth_end         {kcls_conv2_addr_depth_start+kcls_conv2_addr_info_range};
constexpr int kcls_conv2_addr_prkdepth_start    {kcls_conv2_addr_depth_end};
constexpr int kcls_conv2_addr_prkdepth_end      {kcls_conv2_addr_prkdepth_start+kcls_conv2_addr_info_range};
constexpr int kcls_conv2_addr_data_start        {kcls_conv2_addr_prkdepth_end};
constexpr int kcls_conv2_addr_data_end          {kcls_conv2_addr_data_start+kcls_conv2_bundle_num*kcls_conv2_addr_interval_of_data};
constexpr int kcls_conv2_addr_prk_start         {kcls_conv2_addr_data_end};
constexpr int kcls_conv2_addr_prk_end           {kcls_conv2_addr_prk_start + kcls_conv2_bundle_num*kcls_conv2_addr_interval_of_prk};
constexpr int kcls_conv2_addr_bias_start        {kcls_conv2_addr_prk_end};
constexpr int kcls_conv2_addr_bias_end          {kcls_conv2_addr_bias_start + kcls_conv2_kernel_num/(kDramAccessBytes/2)};
constexpr int kcls_conv2_addr_end               {kcls_conv2_addr_bias_end};


constexpr int kcls_conv3_addr_s_start           {kcls_conv2_addr_end};
constexpr int kcls_conv3_addr_s_end             {kcls_conv3_addr_s_start+kcls_conv3_addr_info_range};
constexpr int kcls_conv3_addr_depth_start       {kcls_conv3_addr_s_end};
constexpr int kcls_conv3_addr_depth_end         {kcls_conv3_addr_depth_start+kcls_conv3_addr_info_range};
constexpr int kcls_conv3_addr_prkdepth_start    {kcls_conv3_addr_depth_end};
constexpr int kcls_conv3_addr_prkdepth_end      {kcls_conv3_addr_prkdepth_start+kcls_conv3_addr_info_range};
constexpr int kcls_conv3_addr_data_start        {kcls_conv3_addr_prkdepth_end};
constexpr int kcls_conv3_addr_data_end          {kcls_conv3_addr_data_start+kcls_conv3_bundle_num*kcls_conv3_addr_interval_of_data};
constexpr int kcls_conv3_addr_prk_start         {kcls_conv3_addr_data_end};
constexpr int kcls_conv3_addr_prk_end           {kcls_conv3_addr_prk_start + kcls_conv3_bundle_num*kcls_conv3_addr_interval_of_prk};
constexpr int kcls_conv3_addr_bias_start        {kcls_conv3_addr_prk_end};
constexpr int kcls_conv3_addr_bias_end          {kcls_conv3_addr_bias_start + kcls_conv3_kernel_num/(kDramAccessBytes/2)};
constexpr int kcls_conv3_addr_end               {kcls_conv3_addr_bias_end};

//bbox

constexpr int kbbox_conv1_addr_s_start           {kcls_conv3_addr_end};
constexpr int kbbox_conv1_addr_s_end             {kbbox_conv1_addr_s_start+kbbox_conv1_addr_info_range};
constexpr int kbbox_conv1_addr_depth_start       {kbbox_conv1_addr_s_end};
constexpr int kbbox_conv1_addr_depth_end         {kbbox_conv1_addr_depth_start+kbbox_conv1_addr_info_range};
constexpr int kbbox_conv1_addr_prkdepth_start    {kbbox_conv1_addr_depth_end};
constexpr int kbbox_conv1_addr_prkdepth_end      {kbbox_conv1_addr_prkdepth_start+kbbox_conv1_addr_info_range};
constexpr int kbbox_conv1_addr_data_start        {kbbox_conv1_addr_prkdepth_end};
constexpr int kbbox_conv1_addr_data_end          {kbbox_conv1_addr_data_start+kbbox_conv1_bundle_num*kbbox_conv1_addr_interval_of_data};
constexpr int kbbox_conv1_addr_prk_start         {kbbox_conv1_addr_data_end};
constexpr int kbbox_conv1_addr_prk_end           {kbbox_conv1_addr_prk_start + kbbox_conv1_bundle_num*kbbox_conv1_addr_interval_of_prk};
constexpr int kbbox_conv1_addr_bias_start        {kbbox_conv1_addr_prk_end};
constexpr int kbbox_conv1_addr_bias_end          {kbbox_conv1_addr_bias_start + kbbox_conv1_kernel_num/(kDramAccessBytes/2)};
constexpr int kbbox_conv1_addr_end               {kbbox_conv1_addr_bias_end};


constexpr int kbbox_conv2_addr_s_start           {kbbox_conv1_addr_end};
constexpr int kbbox_conv2_addr_s_end             {kbbox_conv2_addr_s_start+kbbox_conv2_addr_info_range};
constexpr int kbbox_conv2_addr_depth_start       {kbbox_conv2_addr_s_end};
constexpr int kbbox_conv2_addr_depth_end         {kbbox_conv2_addr_depth_start+kbbox_conv2_addr_info_range};
constexpr int kbbox_conv2_addr_prkdepth_start    {kbbox_conv2_addr_depth_end};
constexpr int kbbox_conv2_addr_prkdepth_end      {kbbox_conv2_addr_prkdepth_start+kbbox_conv2_addr_info_range};
constexpr int kbbox_conv2_addr_data_start        {kbbox_conv2_addr_prkdepth_end};
constexpr int kbbox_conv2_addr_data_end          {kbbox_conv2_addr_data_start+kbbox_conv2_bundle_num*kbbox_conv2_addr_interval_of_data};
constexpr int kbbox_conv2_addr_prk_start         {kbbox_conv2_addr_data_end};
constexpr int kbbox_conv2_addr_prk_end           {kbbox_conv2_addr_prk_start + kbbox_conv2_bundle_num*kbbox_conv2_addr_interval_of_prk};
constexpr int kbbox_conv2_addr_bias_start        {kbbox_conv2_addr_prk_end};
constexpr int kbbox_conv2_addr_bias_end          {kbbox_conv2_addr_bias_start + kbbox_conv2_kernel_num/(kDramAccessBytes/2)};
constexpr int kbbox_conv2_addr_end               {kbbox_conv2_addr_bias_end};



constexpr int kbbox_conv3_addr_s_start           {kbbox_conv2_addr_end};
constexpr int kbbox_conv3_addr_s_end             {kbbox_conv3_addr_s_start+kbbox_conv3_addr_info_range};
constexpr int kbbox_conv3_addr_depth_start       {kbbox_conv3_addr_s_end};
constexpr int kbbox_conv3_addr_depth_end         {kbbox_conv3_addr_depth_start+kbbox_conv3_addr_info_range};
constexpr int kbbox_conv3_addr_prkdepth_start    {kbbox_conv3_addr_depth_end};
constexpr int kbbox_conv3_addr_prkdepth_end      {kbbox_conv3_addr_prkdepth_start+kbbox_conv3_addr_info_range};
constexpr int kbbox_conv3_addr_data_start        {kbbox_conv3_addr_prkdepth_end};
constexpr int kbbox_conv3_addr_data_end          {kbbox_conv3_addr_data_start+kbbox_conv3_bundle_num*kbbox_conv3_addr_interval_of_data};
constexpr int kbbox_conv3_addr_prk_start         {kbbox_conv3_addr_data_end};
constexpr int kbbox_conv3_addr_prk_end           {kbbox_conv3_addr_prk_start + kbbox_conv3_bundle_num*kbbox_conv3_addr_interval_of_prk};
constexpr int kbbox_conv3_addr_bias_start        {kbbox_conv3_addr_prk_end};
constexpr int kbbox_conv3_addr_bias_end          {kbbox_conv3_addr_bias_start + kbbox_conv3_kernel_num/(kDramAccessBytes/2)};
constexpr int kbbox_conv3_addr_end               {kbbox_conv3_addr_bias_end};


// output

constexpr int kcls_score_addr_s_start           {kbbox_conv3_addr_end};
constexpr int kcls_score_addr_s_end             {kcls_score_addr_s_start+kcls_score_addr_info_range};
constexpr int kcls_score_addr_depth_start       {kcls_score_addr_s_end};
constexpr int kcls_score_addr_depth_end         {kcls_score_addr_depth_start+kcls_score_addr_info_range};
constexpr int kcls_score_addr_prkdepth_start    {kcls_score_addr_depth_end};
constexpr int kcls_score_addr_prkdepth_end      {kcls_score_addr_prkdepth_start+kcls_score_addr_info_range};
constexpr int kcls_score_addr_data_start        {kcls_score_addr_prkdepth_end};
constexpr int kcls_score_addr_data_end          {kcls_score_addr_data_start+kcls_score_bundle_num*kcls_score_addr_interval_of_data};
constexpr int kcls_score_addr_prk_start         {kcls_score_addr_data_end};
constexpr int kcls_score_addr_prk_end           {kcls_score_addr_prk_start + kcls_score_bundle_num*kcls_score_addr_interval_of_prk};
constexpr int kcls_score_addr_bias_start        {kcls_score_addr_prk_end};
constexpr int kcls_score_addr_bias_end          {1 + kcls_score_addr_bias_start + kcls_score_kernel_num/(kDramAccessBytes/2)};
constexpr int kcls_score_addr_end               {kcls_score_addr_bias_end};



constexpr int kctr_score_addr_s_start           {kcls_score_addr_end};
constexpr int kctr_score_addr_s_end             {kctr_score_addr_s_start+kctr_score_addr_info_range};
constexpr int kctr_score_addr_depth_start       {kctr_score_addr_s_end};
constexpr int kctr_score_addr_depth_end         {kctr_score_addr_depth_start+kctr_score_addr_info_range};
constexpr int kctr_score_addr_prkdepth_start    {kctr_score_addr_depth_end};
constexpr int kctr_score_addr_prkdepth_end      {kctr_score_addr_prkdepth_start+kctr_score_addr_info_range};
constexpr int kctr_score_addr_data_start        {kctr_score_addr_prkdepth_end};
constexpr int kctr_score_addr_data_end          {kctr_score_addr_data_start+kctr_score_bundle_num*kctr_score_addr_interval_of_data};
constexpr int kctr_score_addr_prk_start         {kctr_score_addr_data_end};
constexpr int kctr_score_addr_prk_end           {kctr_score_addr_prk_start + kctr_score_bundle_num*kctr_score_addr_interval_of_prk};
constexpr int kctr_score_addr_bias_start        {kctr_score_addr_prk_end};
constexpr int kctr_score_addr_bias_end          {1 + kctr_score_addr_bias_start + kctr_score_kernel_num/(kDramAccessBytes/2)};
constexpr int kctr_score_addr_end               {kctr_score_addr_bias_end};



constexpr int koffset_score_addr_s_start           {kctr_score_addr_end};
constexpr int koffset_score_addr_s_end             {koffset_score_addr_s_start+koffset_score_addr_info_range};
constexpr int koffset_score_addr_depth_start       {koffset_score_addr_s_end};
constexpr int koffset_score_addr_depth_end         {koffset_score_addr_depth_start+koffset_score_addr_info_range};
constexpr int koffset_score_addr_prkdepth_start    {koffset_score_addr_depth_end};
constexpr int koffset_score_addr_prkdepth_end      {koffset_score_addr_prkdepth_start+koffset_score_addr_info_range};
constexpr int koffset_score_addr_data_start        {koffset_score_addr_prkdepth_end};
constexpr int koffset_score_addr_data_end          {koffset_score_addr_data_start+koffset_score_bundle_num*koffset_score_addr_interval_of_data};
constexpr int koffset_score_addr_prk_start         {koffset_score_addr_data_end};
constexpr int koffset_score_addr_prk_end           {koffset_score_addr_prk_start + koffset_score_bundle_num*koffset_score_addr_interval_of_prk};
constexpr int koffset_score_addr_bias_start        {koffset_score_addr_prk_end};
constexpr int koffset_score_addr_bias_end          {1 + koffset_score_addr_bias_start + koffset_score_kernel_num/(kDramAccessBytes/2)};
constexpr int koffset_score_addr_end               {koffset_score_addr_bias_end};





constexpr int ktest_conv_kernel_size            {3};
constexpr int ktest_conv_kernel_depth           {3};
constexpr int ktest_conv_kernel_num             {96};
constexpr int ktest_conv_addr_interval_of_data  {32};
constexpr int ktest_conv_addr_interval_of_prk   {18};
constexpr int ktest_conv_bundle_num             {12};
constexpr int ktest_conv_addr_info_range        {1};

constexpr int ktest_conv_addr_s_start           {1302001};
constexpr int ktest_conv_addr_s_end             {ktest_conv_addr_s_start+ktest_conv_addr_info_range};
constexpr int ktest_conv_addr_depth_start       {ktest_conv_addr_s_end};
constexpr int ktest_conv_addr_depth_end         {ktest_conv_addr_depth_start+ktest_conv_addr_info_range};
constexpr int ktest_conv_addr_prkdepth_start    {ktest_conv_addr_depth_end};
constexpr int ktest_conv_addr_prkdepth_end      {ktest_conv_addr_prkdepth_start+ktest_conv_addr_info_range};
constexpr int ktest_conv_addr_data_start        {ktest_conv_addr_prkdepth_end};
constexpr int ktest_conv_addr_data_end          {ktest_conv_addr_data_start+ktest_conv_bundle_num*ktest_conv_addr_interval_of_data};
constexpr int ktest_conv_addr_prk_start         {ktest_conv_addr_data_end};
constexpr int ktest_conv_addr_prk_end           {ktest_conv_addr_prk_start + ktest_conv_bundle_num*ktest_conv_addr_interval_of_prk};
constexpr int ktest_conv_addr_bias_start        {ktest_conv_addr_prk_end};
constexpr int ktest_conv_addr_bias_end          {ktest_conv_addr_bias_start + ktest_conv_kernel_num/(kDramAccessBytes/2)};
constexpr int ktest_conv_addr_end               {ktest_conv_addr_bias_end};

constexpr int ktest_conv_input_size {127};
constexpr int ktest_conv_output_size {63};

constexpr int koa_ft_map_addr_depth_end_100     {koa_ft_map_addr_depth_start+1+ktest_conv_input_size*ktest_conv_input_size/kDramAccessBytes};
constexpr int koa_ft_map_test_conv_input_interval_of_data        {1+2*ktest_conv_kernel_depth/kDramAccessBytes};
constexpr int koa_ft_map_addr_data_end_100      {koa_ft_map_addr_depth_end_100+ktest_conv_input_size*ktest_conv_input_size*koa_ft_map_test_conv_input_interval_of_data};

constexpr int koa_ft_map_addr_depth_end_101     {1+koa_ft_map_addr_depth_start+1+ktest_conv_output_size*ktest_conv_output_size/kDramAccessBytes};
constexpr int koa_ft_map_test_conv_output_interval_of_data        {2*ktest_conv_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_addr_data_end_101      {koa_ft_map_addr_depth_end_101+ktest_conv_output_size*ktest_conv_output_size*koa_ft_map_test_conv_output_interval_of_data};




/*=========  Weight: Z Kernel  ==========*/

// constexpr int kr_z_kernel_size            {6};
// constexpr int kr_z_kernel_depth           {256};
// constexpr int kr_z_kernel_num             {1};
// constexpr int kr_z_addr_interval_of_data  {2*specs::kWBundleMaxSize/kDramAccessBytes};
// constexpr int kr_z_addr_interval_of_prk   {1+(1+kr_z_kernel_size*kr_z_kernel_num/specs::kWBundleMaxSize)/kDramAccessBytes};
// constexpr int kr_z_bundle_num             {kr_z_kernel_size*(1+kr_z_kernel_size*kr_z_kernel_depth*kr_z_kernel_num/specs::kWBundleMaxSize)};
// constexpr int kr_z_addr_info_range        {1+kr_z_bundle_num/kDramAccessBytes};
// constexpr int kr_z_addr_s_start           {koffset_score_addr_end};
// constexpr int kr_z_addr_s_end             {kr_z_addr_s_start+kr_z_addr_info_range};
// constexpr int kr_z_addr_depth_start       {kr_z_addr_s_end};
// constexpr int kr_z_addr_depth_end         {kr_z_addr_depth_start+kr_z_addr_info_range};
// constexpr int kr_z_addr_prkdepth_start    {kr_z_addr_depth_end};
// constexpr int kr_z_addr_prkdepth_end      {kr_z_addr_prkdepth_start+kr_z_addr_info_range};
// constexpr int kr_z_addr_data_start        {kr_z_addr_prkdepth_end};
// constexpr int kr_z_addr_data_end          {kr_z_addr_data_start+kr_z_bundle_num*kr_z_addr_interval_of_data};
// constexpr int kr_z_addr_prk_start         {kr_z_addr_data_end};
// constexpr int kr_z_addr_prk_end           {kr_z_addr_prk_start+kr_z_bundle_num*kr_z_addr_interval_of_prk};
// constexpr int kr_z_addr_end               {kr_z_addr_prk_end};


// constexpr int kc_z_kernel_size            {6};
// constexpr int kc_z_kernel_depth           {256};
// constexpr int kc_z_kernel_num             {1};
// constexpr int kc_z_addr_interval_of_data  {2*specs::kWBundleMaxSize/kDramAccessBytes};
// constexpr int kc_z_addr_interval_of_prk   {1+(1+kc_z_kernel_size*kc_z_kernel_num/specs::kWBundleMaxSize)/kDramAccessBytes};
// constexpr int kc_z_bundle_num             {kc_z_kernel_size*(1+kc_z_kernel_size*kc_z_kernel_depth*kc_z_kernel_num/specs::kWBundleMaxSize)};
// constexpr int kc_z_addr_info_range        {1+kc_z_bundle_num/kDramAccessBytes};
// constexpr int kc_z_addr_s_start           {kr_z_addr_end};
// constexpr int kc_z_addr_s_end             {kc_z_addr_s_start+kc_z_addr_info_range};
// constexpr int kc_z_addr_depth_start       {kc_z_addr_s_end};
// constexpr int kc_z_addr_depth_end         {kc_z_addr_depth_start+kc_z_addr_info_range};
// constexpr int kc_z_addr_prkdepth_start    {kc_z_addr_depth_end};
// constexpr int kc_z_addr_prkdepth_end      {kc_z_addr_prkdepth_start+kc_z_addr_info_range};
// constexpr int kc_z_addr_data_start        {kc_z_addr_prkdepth_end};
//                                         //kc_z_addr_interval_of_data = 256? //FIXME
// constexpr int kc_z_addr_data_end          {kc_z_addr_data_start+kc_z_bundle_num*kc_z_addr_interval_of_data};
// constexpr int kc_z_addr_prk_start         {kc_z_addr_data_end};
// constexpr int kc_z_addr_prk_end           {kc_z_addr_prk_start+kc_z_bundle_num*kc_z_addr_interval_of_prk};
// constexpr int kc_z_addr_end               {kc_z_addr_prk_end};






// Flexible depth end
constexpr int koa_ft_map_addr_depth_end_0        {1+koa_ft_map_addr_depth_start+127*127/kDramAccessBytes};
constexpr int koa_ft_map_addr_depth_end_1        {1+koa_ft_map_addr_depth_start+63*63/kDramAccessBytes};
constexpr int koa_ft_map_addr_depth_end_2        {1+koa_ft_map_addr_depth_start+61*61/kDramAccessBytes};
constexpr int koa_ft_map_addr_depth_end_3        {1+koa_ft_map_addr_depth_start+30*30/kDramAccessBytes};
constexpr int koa_ft_map_addr_depth_end_4        {koa_ft_map_addr_depth_start+28*28/kDramAccessBytes};
constexpr int koa_ft_map_addr_depth_end_5        {1+koa_ft_map_addr_depth_start+14*14/kDramAccessBytes};
constexpr int koa_ft_map_addr_depth_end_6        {koa_ft_map_addr_depth_start+12*12/kDramAccessBytes};
constexpr int koa_ft_map_addr_depth_end_7        {1+koa_ft_map_addr_depth_start+10*10/kDramAccessBytes};
constexpr int koa_ft_map_addr_depth_end_8        {koa_ft_map_addr_depth_start+8*8/kDramAccessBytes};

constexpr int koa_ft_map_addr_depth_end_11       {1+koa_ft_map_addr_depth_start+303*303/kDramAccessBytes};
constexpr int koa_ft_map_addr_depth_end_12       {1+koa_ft_map_addr_depth_start+151*151/kDramAccessBytes};
constexpr int koa_ft_map_addr_depth_end_13       {1+koa_ft_map_addr_depth_start+149*149/kDramAccessBytes};
constexpr int koa_ft_map_addr_depth_end_14       {1+koa_ft_map_addr_depth_start+74*74/kDramAccessBytes};
constexpr int koa_ft_map_addr_depth_end_15       {koa_ft_map_addr_depth_start+72*72/kDramAccessBytes};
constexpr int koa_ft_map_addr_depth_end_16       {koa_ft_map_addr_depth_start+36*36/kDramAccessBytes};
constexpr int koa_ft_map_addr_depth_end_17       {1+koa_ft_map_addr_depth_start+34*34/kDramAccessBytes};
constexpr int koa_ft_map_addr_depth_end_18       {koa_ft_map_addr_depth_start+32*32/kDramAccessBytes};
constexpr int koa_ft_map_addr_depth_end_19       {1+koa_ft_map_addr_depth_start+30*30/kDramAccessBytes};

// constexpr int koa_ft_map_addr_depth_end_20       {koa_ft_map_addr_depth_start+28*28/kDramAccessBytes};
// constexpr int koa_ft_map_addr_depth_end_21       {koa_ft_map_addr_depth_start+28*28/kDramAccessBytes};

constexpr int koa_ft_map_addr_depth_end_22       {1+koa_ft_map_addr_depth_start+23*23/kDramAccessBytes};
constexpr int koa_ft_map_addr_depth_end_23       {1+koa_ft_map_addr_depth_start+21*21/kDramAccessBytes};
constexpr int koa_ft_map_addr_depth_end_24       {1+koa_ft_map_addr_depth_start+19*19/kDramAccessBytes};
constexpr int koa_ft_map_addr_depth_end_25       {1+koa_ft_map_addr_depth_start+17*17/kDramAccessBytes};
constexpr int koa_ft_map_addr_depth_end_26       {1+koa_ft_map_addr_depth_start+17*17/kDramAccessBytes};
constexpr int koa_ft_map_addr_depth_end_27       {1+koa_ft_map_addr_depth_start+17*17/kDramAccessBytes};


constexpr int koa_ft_map_addr_depth_end_28       {1+koa_ft_map_addr_depth_start+23*23/kDramAccessBytes};
constexpr int koa_ft_map_addr_depth_end_29       {1+koa_ft_map_addr_depth_start+21*21/kDramAccessBytes};
constexpr int koa_ft_map_addr_depth_end_30       {1+koa_ft_map_addr_depth_start+19*19/kDramAccessBytes};
constexpr int koa_ft_map_addr_depth_end_31       {1+koa_ft_map_addr_depth_start+17*17/kDramAccessBytes};
constexpr int koa_ft_map_addr_depth_end_32       {1+koa_ft_map_addr_depth_start+17*17/kDramAccessBytes};

// Flexible data_end
constexpr int koa_ft_map_addr_data_end_0        {koa_ft_map_addr_depth_end_0+127*127*2*1};
constexpr int koa_ft_map_addr_data_end_1        {koa_ft_map_addr_depth_end_1+63*63*2*kconv1_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_addr_data_end_2        {koa_ft_map_addr_depth_end_2+61*61*2*kconv2_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_addr_data_end_3        {koa_ft_map_addr_depth_end_3+30*30*2*kconv2_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_addr_data_end_4        {koa_ft_map_addr_depth_end_4+28*28*2*kconv3_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_addr_data_end_5        {koa_ft_map_addr_depth_end_5+14*14*2*kconv3_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_addr_data_end_6        {koa_ft_map_addr_depth_end_6+12*12*2*kconv4_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_addr_data_end_7        {koa_ft_map_addr_depth_end_7+10*10*2*kconv5_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_addr_data_end_8        {koa_ft_map_addr_depth_end_8+8*8*2*kconv6_kernel_num/kDramAccessBytes};

//z kernel stored at W region
// constexpr int koa_ft_map_addr_data_end_9       {koa_ft_map_addr_depth_end_9+6*6*2*kr_z_k_kernel_num/kDramAccessBytes};
// constexpr int koa_ft_map_addr_data_end_10       {koa_ft_map_addr_depth_end_10+6*6*2*kc_z_k_kernel_num/kDramAccessBytes};
 
constexpr int koa_ft_map_addr_data_end_11       {koa_ft_map_addr_depth_end_11+303*303*2*1};
constexpr int koa_ft_map_addr_data_end_12       {koa_ft_map_addr_depth_end_12+151*151*2*kconv1_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_addr_data_end_13       {koa_ft_map_addr_depth_end_13+149*149*2*kconv2_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_addr_data_end_14       {koa_ft_map_addr_depth_end_14+74*74*2*kconv2_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_addr_data_end_15       {koa_ft_map_addr_depth_end_15+72*72*2*kconv3_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_addr_data_end_16       {koa_ft_map_addr_depth_end_16+36*36*2*kconv3_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_addr_data_end_17       {koa_ft_map_addr_depth_end_17+34*34*2*kconv4_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_addr_data_end_18       {koa_ft_map_addr_depth_end_18+32*32*2*kconv5_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_addr_data_end_19       {koa_ft_map_addr_depth_end_19+30*30*2*kconv6_kernel_num/kDramAccessBytes};

// constexpr int koa_ft_map_addr_data_end_20       {koa_ft_map_addr_depth_end_20+28*28*2*kc_x_kernel_num/kDramAccessBytes};
// constexpr int koa_ft_map_addr_data_end_21       {koa_ft_map_addr_depth_end_21+28*28*2*kr_x_kernel_num/kDramAccessBytes};

constexpr int koa_ft_map_addr_data_end_22       {koa_ft_map_addr_depth_end_22+23*23*2*kc_x_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_addr_data_end_23       {koa_ft_map_addr_depth_end_23+21*21*2*kcls_conv1_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_addr_data_end_24       {koa_ft_map_addr_depth_end_24+19*19*2*kcls_conv2_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_addr_data_end_25       {koa_ft_map_addr_depth_end_25+17*17*2*kcls_conv3_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_addr_data_end_26       {koa_ft_map_addr_depth_end_26+17*17*2*kcls_score_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_addr_data_end_27       {koa_ft_map_addr_depth_end_27+17*17*2*kctr_score_kernel_num/kDramAccessBytes};

constexpr int koa_ft_map_addr_data_end_28       {koa_ft_map_addr_depth_end_28+23*23*2*kr_x_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_addr_data_end_29       {koa_ft_map_addr_depth_end_29+21*21*2*kbbox_conv1_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_addr_data_end_30       {koa_ft_map_addr_depth_end_30+19*19*2*kbbox_conv2_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_addr_data_end_31       {koa_ft_map_addr_depth_end_31+17*17*2*kbbox_conv3_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_addr_data_end_32       {koa_ft_map_addr_depth_end_32+17*17*2*koffset_score_kernel_num/kDramAccessBytes};


// Flexible interval of data
constexpr int koa_ft_map_conv1_interval_of_data        {2*kconv1_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_conv2_interval_of_data        {2*kconv2_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_conv3_interval_of_data        {2*kconv3_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_conv4_interval_of_data        {2*kconv4_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_conv5_interval_of_data        {2*kconv5_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_conv6_interval_of_data        {2*kconv6_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_cls_conv1_interval_of_data    {2*kcls_conv1_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_cls_conv2_interval_of_data    {2*kcls_conv2_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_cls_conv3_interval_of_data    {2*kcls_conv3_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_bbox_conv1_interval_of_data   {2*kbbox_conv1_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_bbox_conv2_interval_of_data   {2*kbbox_conv2_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_bbox_conv3_interval_of_data   {2*kbbox_conv3_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_cls_score_interval_of_data    {1+2*kcls_score_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_ctr_score_interval_of_data    {1+2*kctr_score_kernel_num/kDramAccessBytes};
constexpr int koa_ft_map_offset_score_interval_of_data {1+2*koffset_score_kernel_num/kDramAccessBytes};

#endif // __ADDRESS_CONFIG_H_V1__