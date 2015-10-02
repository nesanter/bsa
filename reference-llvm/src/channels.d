import llvmd.core;

import symbol_table, errors;

// configuration options
bool channel_unsafe;
uint channel_fifo_size = 4;
uint channel_max_listeners = 16;

Type channel_struct_type, channel_ptr_type;
Value listeners_array;

// the channel struct looks like this:
/*
   struct channel {
       int type; // 0 = listener, 1 = transmit, 2 = receive
       int address; // only for listener & transmit
       unsigned int write_index, read_index;
       int fifo [CHANNEL_FIFO_SIZE];
   };

   struct channel * listeners [MAX_LISTENERS];
*/

enum ChannelFieldIndex {
    TYPE = 0,
    ADDRESS = 1,
    WRITE_INDEX = 2,
    READ_INDEX = 3,
    FIFO = 4
}

enum ChannelType {
    LISTEN = 0,
    TRANSMIT = 1,
    RECEIVE = 2
}

void channel_setup(Module current_module) {
    auto chan_type_type = Type.int_type(32);
    auto chan_addr_type = Type.int_type(32);
    auto chan_index_type = Type.int_type(32);
    auto chan_message_type = Type.int_type(32);
    auto chan_fifo_type = Type.array_type(chan_message_type, channel_fifo_size);
    channel_struct_type = Type.struct_type([], false);
    channel_ptr_type = Type.pointer_type(channel_struct_type);

    listeners_array = Value.create_global_variable(current_module, Type.array_type(channel_ptr_type, channel_max_listeners));
}

Value channel_create(Builder builder) {
    auto chan = builder.alloca(channel_struct_type);
    return chan;
}

void channel_set_type(Value chan, ChannelType type, Builder builder) {
    auto gep = builder.struct_gep(chan, ChannelFieldIndex.TYPE);
    builder.store(Value.create_const_int(Type.int_type(32), type), gep);
}

void channel_get_fifo_used(Value chan, Builder builder) {
    auto gep_write = builder.struct_gep(chan, ChannelFieldIndex.WRITE_INDEX);
    auto gep_read = builder.struct_gep(chan, ChannelFieldIndex.READ_INDEX);

    auto diff = builder.sub(gep_write, gep_read);
    auto ltz = builder.icmp_slt(diff, Value.create_const_int(Type.int_type(32), 0));
    auto negdiff = builder.add(diff, Value.create_const_int(Type.int_type(32), channel_fifo_size));
    auto val = builder.select(ltz, negdiff, diff);

    return val;
}

void channel_get_fifo_remaining(Value chan, Builder builder) {
    auto gep_write = builder.struct_gep(chan, ChannelFieldIndex.WRITE_INDEX);
    auto gep_read = builder.struct_gep(chan, ChannelFieldIndex.READ_INDEX);

    auto diff = builder.sub(gep_read, gep_write);
   auto ltz = builder.icmp_slt(diff, Value.create_const_int(Type.int_type(32), 0));
    auto negdiff = builder.add(diff, Value.create_const_int(Type.int_type(32), channel_fifo_size));
    auto val = builder.select(ltz, negdiff, diff);

    return val;

}

void channel_listen_at(Value chan, Value index, Builder builder) {
    
}
