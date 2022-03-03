/******************************************************************************
 * Copyright (c) 2010-2011, Duane Merrill.  All rights reserved.
 * Copyright (c) 2011-2018, NVIDIA CORPORATION.  All rights reserved.
 * Modifications Copyright (c) 2017-2020, Advanced Micro Devices, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the NVIDIA CORPORATION nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NVIDIA CORPORATION BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

#ifndef HIPCUB_ROCPRIM_BLOCK_BLOCK_ADJACENT_DIFFERENCE_HPP_
#define HIPCUB_ROCPRIM_BLOCK_BLOCK_ADJACENT_DIFFERENCE_HPP_

#include "../../../config.hpp"

#include <rocprim/block/block_adjacent_difference.hpp>

BEGIN_HIPCUB_NAMESPACE

template<
    typename T,
    int BLOCK_DIM_X,
    int BLOCK_DIM_Y = 1,
    int BLOCK_DIM_Z = 1,
    int ARCH = HIPCUB_ARCH /* ignored */
>
class BlockAdjacentDifference
    : private ::rocprim::block_adjacent_difference<
        T,
        BLOCK_DIM_X,
        BLOCK_DIM_Y,
        BLOCK_DIM_Z
      >
{
    static_assert(
        BLOCK_DIM_X * BLOCK_DIM_Y * BLOCK_DIM_Z > 0,
        "BLOCK_DIM_X * BLOCK_DIM_Y * BLOCK_DIM_Z must be greater than 0"
    );

    using base_type =
        typename ::rocprim::block_adjacent_difference<
            T,
            BLOCK_DIM_X,
            BLOCK_DIM_Y,
            BLOCK_DIM_Z
        >;

    // Reference to temporary storage (usually shared memory)
    typename base_type::storage_type& temp_storage_;

public:
    using TempStorage = typename base_type::storage_type;

    HIPCUB_DEVICE inline
    BlockAdjacentDifference() : temp_storage_(private_storage())
    {
    }

    HIPCUB_DEVICE inline
    BlockAdjacentDifference(TempStorage& temp_storage) : temp_storage_(temp_storage)
    {
    }

    template<int ITEMS_PER_THREAD, typename FlagT, typename FlagOp>
    HIPCUB_DEVICE inline
    void FlagHeads(FlagT (&head_flags)[ITEMS_PER_THREAD],
                   T (&input)[ITEMS_PER_THREAD],
                   FlagOp flag_op)
    {
        base_type::flag_heads(head_flags, input, flag_op, temp_storage_);
    }

    template<int ITEMS_PER_THREAD, typename FlagT, typename FlagOp>
    HIPCUB_DEVICE inline
    void FlagHeads(FlagT (&head_flags)[ITEMS_PER_THREAD],
                   T (&input)[ITEMS_PER_THREAD],
                   FlagOp flag_op,
                   T tile_predecessor_item)
    {
        base_type::flag_heads(head_flags, tile_predecessor_item, input, flag_op, temp_storage_);
    }

    template<int ITEMS_PER_THREAD, typename FlagT, typename FlagOp>
    HIPCUB_DEVICE inline
    void FlagTails(FlagT (&tail_flags)[ITEMS_PER_THREAD],
                   T (&input)[ITEMS_PER_THREAD],
                   FlagOp flag_op)
    {
        base_type::flag_tails(tail_flags, input, flag_op, temp_storage_);
    }

    template<int ITEMS_PER_THREAD, typename FlagT, typename FlagOp>
    HIPCUB_DEVICE inline
    void FlagTails(FlagT (&tail_flags)[ITEMS_PER_THREAD],
                   T (&input)[ITEMS_PER_THREAD],
                   FlagOp flag_op,
                   T tile_successor_item)
    {
        base_type::flag_tails(tail_flags, tile_successor_item, input, flag_op, temp_storage_);
    }

    template<int ITEMS_PER_THREAD, typename FlagT, typename FlagOp>
    HIPCUB_DEVICE inline
    void FlagHeadsAndTails(FlagT (&head_flags)[ITEMS_PER_THREAD],
                           FlagT (&tail_flags)[ITEMS_PER_THREAD],
                           T (&input)[ITEMS_PER_THREAD],
                           FlagOp flag_op)
    {
        base_type::flag_heads_and_tails(
            head_flags, tail_flags, input,
            flag_op, temp_storage_
        );
    }

    template<int ITEMS_PER_THREAD, typename FlagT, typename FlagOp>
    HIPCUB_DEVICE inline
    void FlagHeadsAndTails(FlagT (&head_flags)[ITEMS_PER_THREAD],
                           FlagT (&tail_flags)[ITEMS_PER_THREAD],
                           T tile_successor_item,
                           T (&input)[ITEMS_PER_THREAD],
                           FlagOp flag_op)
    {
        base_type::flag_heads_and_tails(
            head_flags, tail_flags, tile_successor_item, input,
            flag_op, temp_storage_
        );
    }

    template<int ITEMS_PER_THREAD, typename FlagT, typename FlagOp>
    HIPCUB_DEVICE inline
    void FlagHeadsAndTails(FlagT (&head_flags)[ITEMS_PER_THREAD],
                           T tile_predecessor_item,
                           FlagT (&tail_flags)[ITEMS_PER_THREAD],
                           T (&input)[ITEMS_PER_THREAD],
                           FlagOp flag_op)
    {
        base_type::flag_heads_and_tails(
            head_flags, tile_predecessor_item, tail_flags, input,
            flag_op, temp_storage_
        );
    }

    template<int ITEMS_PER_THREAD, typename FlagT, typename FlagOp>
    HIPCUB_DEVICE inline
    void FlagHeadsAndTails(FlagT (&head_flags)[ITEMS_PER_THREAD],
                           T tile_predecessor_item,
                           FlagT (&tail_flags)[ITEMS_PER_THREAD],
                           T tile_successor_item,
                           T (&input)[ITEMS_PER_THREAD],
                           FlagOp flag_op)
    {
        base_type::flag_heads_and_tails(
            head_flags, tile_predecessor_item, tail_flags, tile_successor_item, input,
            flag_op, temp_storage_
        );
    }

    template <typename OutputT, unsigned int ITEMS_PER_THREAD, typename BinaryFunctionT>
    HIPCUB_DEVICE inline
    void SubtractLeft(T (&input)[ITEMS_PER_THREAD],
                      OutputT (&output)[ITEMS_PER_THREAD],
                      BinaryFunctionT op,
                      typename base_type::storage_type& storage)
    {
        base_type::subtract_left(
            input, output, op, storage
        );
    }

    template <typename OutputT, unsigned int ITEMS_PER_THREAD, typename BinaryFunctionT>
    HIPCUB_DEVICE inline
    void SubtractLeft(T (&input)[ITEMS_PER_THREAD],
                      OutputT (&output)[ITEMS_PER_THREAD],
                      BinaryFunctionT op,
                      typename base_type::storage_type& storage,
                      T tile_predecessor_item)
    {
        base_type::subtract_left(
            input, output, op, tile_predecessor_item, storage.get().left
        );
    }

    template <typename OutputT, unsigned int ITEMS_PER_THREAD, typename BinaryFunctionT>
    HIPCUB_DEVICE inline
    void SubtractLeftPartial(T (&input)[ITEMS_PER_THREAD],
                                OutputT (&output)[ITEMS_PER_THREAD],
                                BinaryFunctionT op,
                                const unsigned int valid_items,
                                typename base_type::storage_type& storage)
    {
        base_type::subtract_left_partial(
            input, output, op, valid_items, storage.get().left
        );
    }

    template <typename OutputT, unsigned int ITEMS_PER_THREAD, typename BinaryFunctionT>
    HIPCUB_DEVICE inline
    void SubtractLeftPartial(T (&input)[ITEMS_PER_THREAD],
                                OutputT (&output)[ITEMS_PER_THREAD],
                                BinaryFunctionT op,
                                const unsigned int valid_items,
                                typename base_type::storage_type& storage,
                                T tile_predecessor_item)
    {
        base_type::subtract_left_partial(
            input, output, op, tile_predecessor_item, valid_items, storage.get().left
        );
    }

    template <typename OutputT, unsigned int ITEMS_PER_THREAD, typename BinaryFunctionT>
    HIPCUB_DEVICE inline
    void SubtractRight(T (&input)[ITEMS_PER_THREAD],
                      OutputT (&output)[ITEMS_PER_THREAD],
                      BinaryFunctionT op,
                      typename base_type::storage_type& storage)
    {
        base_type::subtract_right(
            input, output, op, storage.get().right
        );
    }

    template <typename OutputT, unsigned int ITEMS_PER_THREAD, typename BinaryFunctionT>
    HIPCUB_DEVICE inline
    void SubtractRight(T (&input)[ITEMS_PER_THREAD],
                      OutputT (&output)[ITEMS_PER_THREAD],
                      BinaryFunctionT op,
                      typename base_type::storage_type& storage,
                      T tile_successor_item)
    {
        base_type::subtract_right(
            input, output, op, tile_successor_item, storage.get().right
        );
    }

    template <typename OutputT, unsigned int ITEMS_PER_THREAD, typename BinaryFunctionT>
    HIPCUB_DEVICE inline
    void SubtractRightPartial(T (&input)[ITEMS_PER_THREAD],
                      OutputT (&output)[ITEMS_PER_THREAD],
                      BinaryFunctionT op,
                      const unsigned int valid_items,
                      typename base_type::storage_type& storage)
    {
        base_type::subtract_right(
            input, output, op, valid_items, storage.get().right
        );
    }

private:
    HIPCUB_DEVICE inline
    TempStorage& private_storage()
    {
        HIPCUB_SHARED_MEMORY TempStorage private_storage;
        return private_storage;
    }
};

END_HIPCUB_NAMESPACE

#endif // HIPCUB_ROCPRIM_BLOCK_BLOCK_ADJACENT_DIFFERENCE_HPP_
