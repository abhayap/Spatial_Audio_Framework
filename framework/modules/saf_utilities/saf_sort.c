/*
 * Copyright 2016-2018 Leo McCormack
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

/**
 * @file saf_sort.c
 * @brief Contains some useful sorting functions
 *
 * @author Leo McCormack
 * @date 30.07.2018
 */

#include "saf_utilities.h"

/**
 * Helper struct for sorting a vector of integers using 'qsort'
 */
typedef struct saf_sort_int {
    int val;
    int idx;
}saf_sort_int;

/**
 * Helper struct for sorting a vector of floats using 'qsort'
 */
typedef struct saf_sort_float {
    float val;
    int idx;
}saf_sort_float;

/**
 * Helper struct for sorting a vector of doubles using 'qsort'
 */
typedef struct saf_sort_double {
    double val;
    int idx;
}saf_sort_double;

/**
 * Helper function for sorting a vector of integers using 'qsort' in ascending
 * order
 */
static int cmp_asc_int(const void *a,const void *b) {
    struct saf_sort_int *a1 = (struct saf_sort_int*)a;
    struct saf_sort_int *a2 = (struct saf_sort_int*)b;
    if((*a1).val<(*a2).val)return -1;
    else if((*a1).val>(*a2).val)return 1;
    else return 0;
}

/**
 * Helper function for a sorting vector of integers using 'qsort' in decending
 * order
 */
static int cmp_desc_int(const void *a,const void *b) {
    struct saf_sort_int *a1 = (struct saf_sort_int*)a;
    struct saf_sort_int *a2 = (struct saf_sort_int*)b;
    if((*a1).val>(*a2).val)return -1;
    else if((*a1).val<(*a2).val)return 1;
    else return 0;
}

/**
 * Helper function for a sorting vector of floats using 'qsort' in ascending
 * order
 */
static int cmp_asc_float(const void *a,const void *b) {
    struct saf_sort_float *a1 = (struct saf_sort_float*)a;
    struct saf_sort_float *a2 = (struct saf_sort_float*)b;
    if((*a1).val<(*a2).val)return -1;
    else if((*a1).val>(*a2).val)return 1;
    else return 0;
}

/**
 * Helper function for a sorting vector of floats using 'qsort' in decending
 * order
 */
static int cmp_desc_float(const void *a,const void *b) {
    struct saf_sort_float *a1 = (struct saf_sort_float*)a;
    struct saf_sort_float *a2 = (struct saf_sort_float*)b;
    if((*a1).val>(*a2).val)return -1;
    else if((*a1).val<(*a2).val)return 1;
    else return 0;
}

/**
 * Helper function for a sorting vector of doubles using 'qsort' in ascending
 * order
 */
static int cmp_asc_double(const void *a,const void *b) {
    struct saf_sort_double *a1 = (struct saf_sort_double*)a;
    struct saf_sort_double *a2 = (struct saf_sort_double*)b;
    if((*a1).val<(*a2).val)return -1;
    else if((*a1).val>(*a2).val)return 1;
    else return 0;
}

/**
 * Helper function for a sorting vector of doubles using 'qsort' in decending
 * order
 */
static int cmp_desc_double(const void *a,const void *b) {
    struct saf_sort_double *a1 = (struct saf_sort_double*)a;
    struct saf_sort_double *a2 = (struct saf_sort_double*)b;
    if((*a1).val>(*a2).val)return -1;
    else if((*a1).val<(*a2).val)return 1;
    else return 0;
}

void sorti
(
    int* in_vec,
    int* out_vec,
    int* new_idices,
    int len,
    int descendFLAG
)
{
    int i;
    struct saf_sort_int *data;
    
    data = malloc1d(len*sizeof(saf_sort_int));
    for(i=0;i<len;i++) {
        data[i].val=in_vec[i];
        data[i].idx=i;
    }
    if(descendFLAG)
        qsort(data,len,sizeof(data[0]),cmp_desc_int);
    else
        qsort(data,len,sizeof(data[0]),cmp_asc_int);
    for(i=0;i<len;i++){
        if (out_vec!=NULL)
            out_vec[i] = data[i].val;
        else
            in_vec[i] = data[i].val; /* overwrite input vector */
        if(new_idices!=NULL)
            new_idices[i] = data[i].idx;
    }
    free(data);
}

void sortf
(
    float* in_vec,
    float* out_vec,
    int* new_idices,
    int len,
    int descendFLAG
)
{
    int i;
    struct saf_sort_float *data;
    
    data = malloc1d(len*sizeof(saf_sort_float));
    for(i=0;i<len;i++) {
        data[i].val=in_vec[i];
        data[i].idx=i;
    }
    if(descendFLAG)
        qsort(data,len,sizeof(data[0]),cmp_desc_float);
    else
        qsort(data,len,sizeof(data[0]),cmp_asc_float);
    for(i=0;i<len;i++){
        if (out_vec!=NULL)
            out_vec[i] = data[i].val;
        else
            in_vec[i] = data[i].val; /* overwrite input vector */
        if(new_idices!=NULL)
            new_idices[i] = data[i].idx;
    }
    free(data);
}

void sortd
(
    double* in_vec,
    double* out_vec,
    int* new_idices,
    int len,
    int descendFLAG
)
{
    int i;
    struct saf_sort_double *data;
    
    data = malloc1d(len*sizeof(saf_sort_double));
    for(i=0;i<len;i++) {
        data[i].val=in_vec[i];
        data[i].idx=i;
    }
    if(descendFLAG)
        qsort(data,len,sizeof(data[0]),cmp_desc_double);
    else
        qsort(data,len,sizeof(data[0]),cmp_asc_double);
    for(i=0;i<len;i++){
        if (out_vec!=NULL)
            out_vec[i] = data[i].val;
        else
            in_vec[i] = data[i].val; /* overwrite input vector */
        if(new_idices!=NULL)
            new_idices[i] = data[i].idx;
    }
    free(data);
}

void findClosestGridPoints
(
    float* grid_dirs,
    int nGrid,
    float* target_dirs,
    int nTarget,
    int degFLAG,
    int* idx_closest,
    float* dirs_closest,
    float* angle_diff
)
{
    int i, j;
    float* grid_xyz, *target_xyz;
    float rcoselev, max_val, current_val;
    
    /* convert sph coords into Cartesian coords */
    grid_xyz = malloc1d(nGrid*3*sizeof(float));
    target_xyz = malloc1d(nTarget*3*sizeof(float));
    if(degFLAG){
        for(i=0; i<nGrid; i++){
            grid_xyz[i*3+2] = sinf(grid_dirs[i*2+1]*M_PI/180.0f);
            rcoselev = cosf( grid_dirs[i*2+1]*M_PI/180.0f);
            grid_xyz[i*3] = rcoselev * cosf(grid_dirs[i*2]*M_PI/180.0f);
            grid_xyz[i*3+1] = rcoselev * sinf(grid_dirs[i*2]*M_PI/180.0f);
        }
        for(i=0; i<nTarget; i++){
            target_xyz[i*3+2] = sinf(target_dirs[i*2+1]*M_PI/180.0f);
            rcoselev = cosf(target_dirs[i*2+1]*M_PI/180.0f);
            target_xyz[i*3] = rcoselev * cosf(target_dirs[i*2]*M_PI/180.0f);
            target_xyz[i*3+1] = rcoselev * sinf(target_dirs[i*2]*M_PI/180.0f);
        }
    }
    else{
        for(i=0; i<nGrid; i++){
            grid_xyz[i*3+2] = sinf(grid_dirs[i*2+1]);
            rcoselev = cosf(grid_dirs[i*2+1]);
            grid_xyz[i*3] = rcoselev * cosf(grid_dirs[i*2]);
            grid_xyz[i*3+1] = rcoselev * sinf(grid_dirs[i*2]);
        }
        for(i=0; i<nTarget; i++){
            target_xyz[i*3+2] = sinf(target_dirs[i*2+1]);
            rcoselev = cosf(target_dirs[i*2+1]);
            target_xyz[i*3] = rcoselev * cosf(target_dirs[i*2]);
            target_xyz[i*3+1] = rcoselev * sinf(target_dirs[i*2]);
        }
    }
    
    /* determine which 'grid_dirs' indices are the closest to 'target_dirs' */
    for(i=0; i<nTarget; i++){
        max_val = -2.23e10f;
        for(j=0; j<nGrid; j++){
            current_val = grid_xyz[j*3] * target_xyz[i*3] +
                          grid_xyz[j*3+1] * target_xyz[i*3+1] +
                          grid_xyz[j*3+2] * target_xyz[i*3+2];
            if(current_val > max_val)
                idx_closest[i] = j;
            if(current_val>max_val){
                idx_closest[i] = j;
                max_val = current_val;
                if(angle_diff!=NULL)
                    angle_diff[i] = acosf(max_val);
            }
        }
    }
    
    /* optional output of directions */
    if(dirs_closest!=NULL){
        for(i=0; i<nTarget; i++){
            dirs_closest[i*2] = grid_dirs[idx_closest[i]*2];
            dirs_closest[i*2+1] = grid_dirs[idx_closest[i]*2+1];
        }
    }
    
    free(grid_xyz);
    free(target_xyz);
}










