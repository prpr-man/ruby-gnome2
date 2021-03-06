class TestGtkImage < Test::Unit::TestCase
  include GtkTestUtils

  sub_test_case ".new" do
    test "stock" do
      image = Gtk::Image.new(:stock => Gtk::Stock::GO_FORWARD,
                             :size => :dialog)
      assert_equal([
                     Gtk::Stock::GO_FORWARD.to_s,
                     Gtk::IconSize::DIALOG,
                   ],
                   image.stock)
    end

    test "icon_name" do
      icon_name = "dialog-password"
      image = Gtk::Image.new(:icon_name => icon_name,
                             :size => Gtk::IconSize::DIALOG)
      assert_equal([
                     icon_name,
                     Gtk::IconSize::DIALOG,
                   ],
                   image.icon_name)
    end

    test "icon" do
      icon = Gio::ThemedIcon.new("gtk-open")
      image = Gtk::Image.new(:icon => icon,
                             :size => :dialog)
      assert_equal([
                     icon,
                     Gtk::IconSize::DIALOG,
                   ],
                   image.icon)
    end

    test "resource" do
      resource = Gio::Resource.load(fixture_path("image.gresource"))
      Gio::Resources.register(resource)
      begin
        resource_path = "/org/ruby/gnome/gnome-logo-icon.png"
        image = Gtk::Image.new(:resource => resource_path,
                               :size => :dialog)
        omit("GTK+ 3.16 doesn't support getting resource path. See GitHub#461.")
        assert_equal(resource_path,
                     image.resource)
      ensure
        Gio::Resources.unregister(resource)
      end
    end
  end
end
