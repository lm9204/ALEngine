namespace ALEngine
{
	public class Input
	{
		public static bool isKeyDown(KeyCode keycode)
		{
			return InternalCalls.Input_isKeyDown(keycode);
		}
	}
}
