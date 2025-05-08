# generated from rosidl_generator_py/resource/_idl.py.em
# with input from grpr2f85_ifaces:srv/SetGripperState.idl
# generated code does not contain a copyright notice


# Import statements for member types

import builtins  # noqa: E402, I100

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_SetGripperState_Request(type):
    """Metaclass of message 'SetGripperState_Request'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('grpr2f85_ifaces')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'grpr2f85_ifaces.srv.SetGripperState_Request')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__srv__set_gripper_state__request
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__srv__set_gripper_state__request
            cls._CONVERT_TO_PY = module.convert_to_py_msg__srv__set_gripper_state__request
            cls._TYPE_SUPPORT = module.type_support_msg__srv__set_gripper_state__request
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__srv__set_gripper_state__request

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
            'POSITION__DEFAULT': 0,
            'SPEED__DEFAULT': 255,
            'FORCE__DEFAULT': 255,
            'WAIT_TIME__DEFAULT': 0,
        }

    @property
    def POSITION__DEFAULT(cls):
        """Return default value for message field 'position'."""
        return 0

    @property
    def SPEED__DEFAULT(cls):
        """Return default value for message field 'speed'."""
        return 255

    @property
    def FORCE__DEFAULT(cls):
        """Return default value for message field 'force'."""
        return 255

    @property
    def WAIT_TIME__DEFAULT(cls):
        """Return default value for message field 'wait_time'."""
        return 0


class SetGripperState_Request(metaclass=Metaclass_SetGripperState_Request):
    """Message class 'SetGripperState_Request'."""

    __slots__ = [
        '_position',
        '_speed',
        '_force',
        '_wait_time',
    ]

    _fields_and_field_types = {
        'position': 'uint8',
        'speed': 'uint8',
        'force': 'uint8',
        'wait_time': 'uint16',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.BasicType('uint8'),  # noqa: E501
        rosidl_parser.definition.BasicType('uint8'),  # noqa: E501
        rosidl_parser.definition.BasicType('uint8'),  # noqa: E501
        rosidl_parser.definition.BasicType('uint16'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.position = kwargs.get(
            'position', SetGripperState_Request.POSITION__DEFAULT)
        self.speed = kwargs.get(
            'speed', SetGripperState_Request.SPEED__DEFAULT)
        self.force = kwargs.get(
            'force', SetGripperState_Request.FORCE__DEFAULT)
        self.wait_time = kwargs.get(
            'wait_time', SetGripperState_Request.WAIT_TIME__DEFAULT)

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.__slots__, self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s[1:] + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.position != other.position:
            return False
        if self.speed != other.speed:
            return False
        if self.force != other.force:
            return False
        if self.wait_time != other.wait_time:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def position(self):
        """Message field 'position'."""
        return self._position

    @position.setter
    def position(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'position' field must be of type 'int'"
            assert value >= 0 and value < 256, \
                "The 'position' field must be an unsigned integer in [0, 255]"
        self._position = value

    @builtins.property
    def speed(self):
        """Message field 'speed'."""
        return self._speed

    @speed.setter
    def speed(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'speed' field must be of type 'int'"
            assert value >= 0 and value < 256, \
                "The 'speed' field must be an unsigned integer in [0, 255]"
        self._speed = value

    @builtins.property
    def force(self):
        """Message field 'force'."""
        return self._force

    @force.setter
    def force(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'force' field must be of type 'int'"
            assert value >= 0 and value < 256, \
                "The 'force' field must be an unsigned integer in [0, 255]"
        self._force = value

    @builtins.property
    def wait_time(self):
        """Message field 'wait_time'."""
        return self._wait_time

    @wait_time.setter
    def wait_time(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'wait_time' field must be of type 'int'"
            assert value >= 0 and value < 65536, \
                "The 'wait_time' field must be an unsigned integer in [0, 65535]"
        self._wait_time = value


# Import statements for member types

# already imported above
# import builtins

# already imported above
# import rosidl_parser.definition


class Metaclass_SetGripperState_Response(type):
    """Metaclass of message 'SetGripperState_Response'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('grpr2f85_ifaces')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'grpr2f85_ifaces.srv.SetGripperState_Response')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__srv__set_gripper_state__response
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__srv__set_gripper_state__response
            cls._CONVERT_TO_PY = module.convert_to_py_msg__srv__set_gripper_state__response
            cls._TYPE_SUPPORT = module.type_support_msg__srv__set_gripper_state__response
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__srv__set_gripper_state__response

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
            'OK__DEFAULT': False,
            'RESULT__DEFAULT': '',
        }

    @property
    def OK__DEFAULT(cls):
        """Return default value for message field 'ok'."""
        return False

    @property
    def RESULT__DEFAULT(cls):
        """Return default value for message field 'result'."""
        return ''


class SetGripperState_Response(metaclass=Metaclass_SetGripperState_Response):
    """Message class 'SetGripperState_Response'."""

    __slots__ = [
        '_ok',
        '_status_code',
        '_result',
    ]

    _fields_and_field_types = {
        'ok': 'boolean',
        'status_code': 'uint8',
        'result': 'string',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.BasicType('boolean'),  # noqa: E501
        rosidl_parser.definition.BasicType('uint8'),  # noqa: E501
        rosidl_parser.definition.UnboundedString(),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.ok = kwargs.get(
            'ok', SetGripperState_Response.OK__DEFAULT)
        self.status_code = kwargs.get('status_code', int())
        self.result = kwargs.get(
            'result', SetGripperState_Response.RESULT__DEFAULT)

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.__slots__, self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s[1:] + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.ok != other.ok:
            return False
        if self.status_code != other.status_code:
            return False
        if self.result != other.result:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def ok(self):
        """Message field 'ok'."""
        return self._ok

    @ok.setter
    def ok(self, value):
        if __debug__:
            assert \
                isinstance(value, bool), \
                "The 'ok' field must be of type 'bool'"
        self._ok = value

    @builtins.property
    def status_code(self):
        """Message field 'status_code'."""
        return self._status_code

    @status_code.setter
    def status_code(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'status_code' field must be of type 'int'"
            assert value >= 0 and value < 256, \
                "The 'status_code' field must be an unsigned integer in [0, 255]"
        self._status_code = value

    @builtins.property
    def result(self):
        """Message field 'result'."""
        return self._result

    @result.setter
    def result(self, value):
        if __debug__:
            assert \
                isinstance(value, str), \
                "The 'result' field must be of type 'str'"
        self._result = value


class Metaclass_SetGripperState(type):
    """Metaclass of service 'SetGripperState'."""

    _TYPE_SUPPORT = None

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('grpr2f85_ifaces')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'grpr2f85_ifaces.srv.SetGripperState')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._TYPE_SUPPORT = module.type_support_srv__srv__set_gripper_state

            from grpr2f85_ifaces.srv import _set_gripper_state
            if _set_gripper_state.Metaclass_SetGripperState_Request._TYPE_SUPPORT is None:
                _set_gripper_state.Metaclass_SetGripperState_Request.__import_type_support__()
            if _set_gripper_state.Metaclass_SetGripperState_Response._TYPE_SUPPORT is None:
                _set_gripper_state.Metaclass_SetGripperState_Response.__import_type_support__()


class SetGripperState(metaclass=Metaclass_SetGripperState):
    from grpr2f85_ifaces.srv._set_gripper_state import SetGripperState_Request as Request
    from grpr2f85_ifaces.srv._set_gripper_state import SetGripperState_Response as Response

    def __init__(self):
        raise NotImplementedError('Service classes can not be instantiated')
