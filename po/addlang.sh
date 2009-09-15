
echo "add_subdirectory($1)" >> CMakeLists.txt

mkdir $1 -p
cat > $1/CMakeLists.txt <<EOF
GETTEXT_PROCESS_PO_FILES($1 ALL
			INSTALL_DESTINATION \${LOCALE_INSTALL_DIR}
			kamoso.po )
EOF
