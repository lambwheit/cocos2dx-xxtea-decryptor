--[[
Description: 禁言tableview item
Author: dashuaibi
Date: 2020-08-04 15:22:28
App: X-Poker
--]]
local ban_talk_item = class("ban_talk_item", base_widget)


function ban_talk_item:ctor( params )
	ban_talk_item.super:ctor(self, params)
    self:set_data(params.data, params.index)
    self.click_callback = params.click_callback
    self:setNodeEventEnabled(true)

    self.sub_node_params = {
        head_icon = {cls = "wd_user_icon"}
    }
end


function ban_talk_item:while_set_root()
    self.head_icon = self.rootNode._children["head_icon"]
    self.lab_name = self.rootNode._children["lab_name"]
    self.btn_ban = self.rootNode._children["btn_ban"]
    self.btn_unban = self.rootNode._children["btn_unban"]

	self:refresh_ui()
end


function ban_talk_item:on_click(  )
	local param = {
		uid = self.data.uid,
		ban = not self.data.is_ban_talk,
	}

    g_protocol:send_msg_BanChatC2S(param, nil, self.data.room_id)
end

function ban_talk_item:refresh_ui(  )
    self.rootNode._children["bg"]:setVisible( self.index % 2 == 0 )
    self.head_icon.controller:update_head_info(self.data.avatar, self.data.vip_level)

    local name = g_language:get_string_by_len( self.data.name, 26, 230 )
    self.lab_name:setString(name)

    self:refresh_btn_icon()
end

function ban_talk_item:refresh_btn_icon(  )
    self.btn_ban:setVisible(not self.data.is_ban_talk)
    self.btn_unban:setVisible(self.data.is_ban_talk)
end

function ban_talk_item:set_data( data, index )
    self.data = data
    self.index = index
end

function ban_talk_item:refreshContentView( data, index )
    self:set_data(data, index)
    self:refresh_ui()
end

function ban_talk_item:on_update_ban_status( event )
    local params = event.params
	if params.uid == self.data.uid then
		self.data.is_ban_talk = params.ban
		self:refresh_btn_icon()
	end
end

function ban_talk_item:addEventListeners()
    self._updateForbidState = handler(self, self.on_update_ban_status)
    g_event_center:addEventListener(g_custom_event.EVT_PLAYER_FORBID, self._updateForbidState)
end

function ban_talk_item:removeEventListeners()
    g_event_center:removeEventListener(g_custom_event.EVT_PLAYER_FORBID, self._updateForbidState)
end

return ban_talk_item