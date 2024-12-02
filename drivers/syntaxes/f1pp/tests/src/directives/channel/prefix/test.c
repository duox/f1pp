
#block	// begin local definitions

// Define input data
#set CMDS = {
	{ cmd_close,	"close" },
	{ cmd_define,	"define" }
}

// Define channels
#channel.define CHANNEL_ENUM_IDS
#channel.header CHANNEL_ENUM_IDS	enum cmd_id_t {
#channel.footer CHANNEL_ENUM_IDS	};

#channel.define CHANNEL_NAMES
#channel.header CHANNEL_NAMES		static const char * const cmd_names[] = {
#channel.footer CHANNEL_NAMES		};

#channel.define CHANNEL_LENGTHS
#channel.header CHANNEL_LENGTHS		static const uint8_t	cmd_lens[] = {
#channel.footer CHANNEL_LENGTHS		};

#channel.define CHANNEL_IDS
#channel.header CHANNEL_IDS			static const uint8_t	cmd_ids[] = {
#channel.footer CHANNEL_IDS			};

// Generate data in corresponding channels
#foreach [ cmd, str ]: CMDS
	#channel	CHANNEL_ENUM_IDS
	cmd_##cmd,
	#channel	CHANNEL_NAMES
	str,
	#channel	CHANNEL_LENGTHS
	sizeof(str) - 1,
	#channel	CHANNEL_IDS
	cmd_##cmd,
#endforeach

// Place data in the source file
CHANNEL_ENUM_IDS
CHANNEL_NAMES
CHANNEL_LENGTHS
CHANNEL_IDS

#endblock
