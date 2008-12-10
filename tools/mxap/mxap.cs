
using NDesk.Options;
using System;
using System.IO;
using System.Text;
using System.Diagnostics;
using System.Collections.Generic;

namespace Moonlight {

	public class MXap {

		private string application_name;
		private List<string> external_assemblies;
		private List<string> reference_assemblies;
		private List<string> mdb_files;
		private List<string> csharp_files;
		private List<string> xaml_files;
		private string top_builddir = null; // Defaults to null
		private bool desktop = false; // Defaults to false
		private bool generate_html = true; // Defaults to true
		private bool include_mdb = true; // Defaults to true
		private bool verbose = false; //default to false
		private bool generate_manifest = true; 
		private bool list_generated = false;
		private string entry_point_type = null;
		private string cs_sources;

		const string RuntimeVersion = "2.0.31005.0";

		public string CSSources {
			get { return cs_sources; }
			set { cs_sources = value; }
		}
		
		public string EntryPointType {
			get { return entry_point_type; }
			set { entry_point_type = value;  }
		}
		
		public bool GenerateManifest {
			get { return generate_manifest; }
			set { generate_manifest = value; }
		}
		
		public bool GenerateHtml {
			get { return generate_html; }
			set { generate_html = value; }
		}

		public bool Desktop {
			get { return desktop; }
			set { desktop = value; }
		}

		public string TopBuildDir {
			get { return top_builddir; }
			set { top_builddir = value; }
		}

		public bool IncludeMdb {
			get { return include_mdb; }
			set { include_mdb = value; }
		}

		public bool Verbose {
			get { return verbose; }
			set { verbose = value; }
		}

		public bool ListGenerated {
			get { return list_generated; }
			set { list_generated = value; }
		}
		
		public string ApplicationName {
			get {
				if (application_name == null) {
					DirectoryInfo di = new DirectoryInfo (Directory.GetCurrentDirectory ());
					application_name = di.Name;
					application_name.Replace ("-", "_");
				}

				return application_name;
			}
			set {
				application_name = value;
			}
		}

		public List<string> ReferenceAssemblies {
			get {
				if (reference_assemblies == null)
					reference_assemblies = new List<string> ();
				return reference_assemblies;
			}
		}

		public List<string> ExternalAssemblies {
			get {
				if (external_assemblies == null)
					external_assemblies = new List<string> ();
				return external_assemblies;
			}
		}
		
		public List<string> MdbFiles {
			get {
				if (mdb_files == null)
					mdb_files = new List<string> ();
				return mdb_files;
			}
		}

		public List<string> CSharpFiles {
			get {
				if (csharp_files == null)
					csharp_files = new List<string> ();
				return csharp_files;
			}
		}

		public List<string> XamlFiles {
			get {
				if (xaml_files == null)
					xaml_files = new List<string> ();
				return xaml_files;
			}
		}

		public int Run ()
		{
			return (CreateManifest ()
				&& CreateCodeBehind ()
				&& CreateResources ()
				&& CreateApplicationAssembly ()
				&& CreateXap ()
				&& CreateHtmlWrapper ()) ? 0 : 1;
		}

		public bool CreateManifest ()
		{
			string AppManifest_Filename = "AppManifest.xaml";

			if (!GenerateManifest)
				return true;
			
			if (ListGenerated) {
				Console.WriteLine (AppManifest_Filename);
				return true;
			}

			StringBuilder manifest = new StringBuilder ();

			manifest.Append ("<Deployment xmlns=\"http://schemas.microsoft.com/client/2007/deployment\"");
			manifest.Append (" xmlns:x=\"http://schemas.microsoft.com/winfx/2006/xaml\" ");
			manifest.AppendFormat ("EntryPointAssembly=\"{0}\" EntryPointType=\"", ApplicationName);
			if (entry_point_type != null)
				manifest.Append (entry_point_type);
			else 
				manifest.AppendFormat ("{0}.App", ApplicationName);
			manifest.AppendFormat ("\" RuntimeVersion=\"{0}\">\n", RuntimeVersion);

			manifest.AppendLine ("  <Deployment.Parts>");

			foreach (string assembly in ReferenceAssemblies) {
				manifest.AppendFormat ("    <AssemblyPart x:Name=\"{0}\" Source=\"{1}\" />\n", Path.GetFileNameWithoutExtension (assembly), Path.GetFileName (assembly));
			}
			manifest.AppendFormat ("    <AssemblyPart x:Name=\"{0}\" Source=\"{1}.dll\" />\n", ApplicationName, ApplicationName);
			
			manifest.AppendLine ("  </Deployment.Parts>");
			manifest.AppendLine ("</Deployment>");

			File.WriteAllText (AppManifest_Filename, manifest.ToString ());

			return true;
		}

		public bool CreateCodeBehind ()
		{
			if (ListGenerated) {
				foreach (string xaml_file in XamlFiles) {
					if (Path.GetFileName (xaml_file) == "AppManifest.xaml")
						continue;
					Console.WriteLine ("{0}.g.cs", Path.GetFileName (xaml_file));
				}
				return true;
			}
			else {
				StringBuilder xamlg_args = new StringBuilder ();

				xamlg_args.AppendFormat (" -sl2app:{0} ", ApplicationName);

				foreach (string xaml_file in XamlFiles) {
					if (Path.GetFileName (xaml_file) == "AppManifest.xaml")
						continue;
					//xamlg_args.AppendFormat (" {0}", Path.GetFileName (xaml_file));
					xamlg_args.AppendFormat (" \"{0}\"", xaml_file);
				}

				return RunTool ("xamlg",
						"tools/xamlg/xamlg.exe",
						xamlg_args.ToString ());
			}
		}

		public bool CreateResources ()
		{
			if (ListGenerated) {
				Console.WriteLine ("{0}.g.resources", ApplicationName);
				return true;
			}

			StringBuilder respack_args = new StringBuilder ();

			respack_args.AppendFormat ("{0}.g.resources ", ApplicationName);

			foreach (string xaml_file in XamlFiles) {
				if (Path.GetFileName (xaml_file) == "AppManifest.xaml")
					continue;
				//respack_args.AppendFormat (" {0}", Path.GetFileName (xaml_file));
				respack_args.AppendFormat (" \"{0}\"", xaml_file);
			}

			return RunTool ("respack",
					"tools/respack/respack.exe",
					respack_args.ToString ());
		}

		public bool CreateApplicationAssembly ()
		{
			if (ListGenerated) {
				Console.WriteLine ("{0}.dll", ApplicationName);
				return true;
			}

			StringBuilder compiler_args = new StringBuilder ();

			compiler_args.AppendFormat (" -debug+ -target:library -out:{0}.dll ", ApplicationName);

			foreach (string asm in ReferenceAssemblies) {
				compiler_args.AppendFormat (" -r:\"{0}\" ", asm);
			}
			
			foreach (string asm in ExternalAssemblies) {
				compiler_args.AppendFormat (" -r:{0} ", asm);
			}

			if (desktop && top_builddir == null) {
				compiler_args.Append (" -pkg:silverdesktop ");
				compiler_args.Append (" -pkg:gtksilver ");
			}

			foreach (string cs in CSharpFiles) {
				if (cs.EndsWith (".g.cs"))
					continue;
				compiler_args.AppendFormat (" \"{0}\" ", cs);
			}

			foreach (string xaml in XamlFiles) {
				if (Path.GetFileName (xaml) == "AppManifest.xaml")
					continue;
				if (!File.Exists (Path.GetFileName (xaml) + ".g.cs"))
					continue;
				compiler_args.AppendFormat (" {0}.g.cs ", Path.GetFileName (xaml));
			}

			compiler_args.AppendFormat (" -resource:{0}.g.resources ", ApplicationName);

			if (desktop)
				return RunProcess ("gmcs", compiler_args.ToString());
			else
				return RunTool ("smcs",
						"class/lib/2.1/smcs.exe",
						compiler_args.ToString());
		}

		public bool CreateXap ()
		{
			if (ListGenerated) {
				Console.WriteLine ("{0}.xap", ApplicationName);
				return true;
			}

			StringBuilder zip_args = new StringBuilder ();

			zip_args.AppendFormat (" {0}.xap ", ApplicationName);

			foreach (string asm in ReferenceAssemblies) {
				zip_args.AppendFormat (" {0} ", Path.GetFileName (asm));
			}
			foreach (string mdb in MdbFiles) {
				zip_args.AppendFormat (" {0} ", Path.GetFileName (mdb));
			}
			
			zip_args.AppendFormat (" AppManifest.xaml ");
			zip_args.AppendFormat (" {0}.dll ", ApplicationName);

			return RunProcess ("zip", zip_args.ToString ());
			
		}

		public bool CreateHtmlWrapper ()
		{
			if (!GenerateHtml)
				return true;

			if (ListGenerated) {
				Console.WriteLine ("{0}.html", ApplicationName);
				return true;
			}

			StringBuilder xaml2html_args = new StringBuilder ();

			xaml2html_args.AppendFormat (" {0}.xap ", ApplicationName);

			return RunTool ("xaml2html",
					"tools/xaml2html/xaml2html.exe",
					xaml2html_args.ToString ());
		}

		private bool RunTool (string filename, string builddir_exe, string args)
		{
			if (top_builddir != null) {
				bool need_smcs_hack = false;
				if (filename == "smcs")
					need_smcs_hack = true;
				filename = "mono";
				args = String.Format ("{3}{0}/{1} {2}", top_builddir, builddir_exe, args,
						      need_smcs_hack ? "--runtime=moonlight --security=temporary-smcs-hack " : "");
			}

			return RunProcess (filename, args);
		}

		private bool RunProcess (string filename, string args)
		{
			if (Verbose)
				Console.WriteLine ("Running {0} {1}", filename, args);

			Process process = new Process ();

			process.StartInfo.FileName = filename;
			process.StartInfo.Arguments = args;

			process.StartInfo.CreateNoWindow = true;
			process.StartInfo.UseShellExecute = false;

			process.Start ();

			process.WaitForExit ();

			return (process.ExitCode == 0);
		}

		static void ShowHelp (OptionSet os)
		{
			Console.WriteLine ("mxap usage is: mxap [options] [directory]");
			Console.WriteLine ();
			os.WriteOptionDescriptions (Console.Out);
		}

		public static int Main (string [] args)
		{
			MXap mxap = new MXap ();
			bool help = false;
			string cd = Directory.GetCurrentDirectory ();
			
			var p = new OptionSet () {
				{ "h|?|help", v => help = v != null },
				{ "generate-html", v => mxap.GenerateHtml = v != null },
				{ "include-mdb", v => mxap.IncludeMdb = v != null},
				{ "application-name=", v => mxap.ApplicationName = v },
				{ "generate-manifest", v => mxap.GenerateManifest = v != null },
				{ "entry-point-type=", v => mxap.EntryPointType = v },
				{ "cs-sources=", v => mxap.CSSources = v },
				{ "desktop", v => mxap.Desktop = v != null },
				{ "builddirhack=", v => mxap.TopBuildDir = v },
				{ "r:|reference:", v => mxap.ExternalAssemblies.Add (v) },
				{ "l|list-generated", v => mxap.ListGenerated = v != null },
				{ "v|verbose", v => mxap.Verbose =  v != null }
			};

			List<string> extra = null;
			try {
				extra = p.Parse(args);
			} catch (OptionException){
				Console.WriteLine ("Try `mxap --help' for more information.");
				return 1;
			}

			if (help){
				ShowHelp (p);
				return 0;
			}

			if (extra.Count > 0)
				cd = extra [0];

			if (mxap.TopBuildDir == null && mxap.ExternalAssemblies.Count > 0) {
				Console.Error.WriteLine ("--reference requires --builddirhack");
				return 1;
			}

			mxap.ReferenceAssemblies.AddRange (Directory.GetFiles (cd, "*.dll"));
			mxap.XamlFiles.AddRange (Directory.GetFiles (cd, "*.xaml"));
			if (mxap.CSSources == null) {
				mxap.CSharpFiles.AddRange (Directory.GetFiles (cd, "*.cs"));
			} else {
				mxap.CSharpFiles.AddRange (File.ReadAllLines (mxap.CSSources));
			}
			
			if (mxap.IncludeMdb)
				mxap.MdbFiles.AddRange (Directory.GetFiles (cd, "*.mdb"));

			if (mxap.XamlFiles.Count == 0 || mxap.CSharpFiles.Count == 0) {
				Console.Error.WriteLine ("No XAML files or C# files found");
				ShowHelp (p);
				return 1;
			}

			// Make sure we didn't add the Application assembly into the referenced assemblies
			DirectoryInfo info = new DirectoryInfo (cd);

			if (mxap.ReferenceAssemblies.Contains (Path.Combine (cd, info.Name + ".dll")))
				mxap.ReferenceAssemblies.Remove (Path.Combine (cd, info.Name + ".dll"));


			return mxap.Run ();
		}
	}
}


